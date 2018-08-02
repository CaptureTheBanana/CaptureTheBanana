// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <limits>

#include <boost/filesystem.hpp>
#include <parser/GameParser.hpp>
#include <parser/LevelParser.hpp>

#include "editor/model/MainWindowModel.hpp"
#include "editor/scene/GraphicsItems.hpp"
#include "editor/scene/GraphicsScenes.hpp"

namespace ctb {
namespace editor {

MainWindowModel::MainWindowModel(GraphicsTilesetItem::CallbackT&& tilesetCallback)
    : m_tilesetItemCallback(std::forward<GraphicsTilesetItem::CallbackT>(tilesetCallback)) {}

// ----------------------------------------------------------------------------

void MainWindowModel::reset() {
    m_hasChanges = false;

    m_tilesetScenes.clear();
    m_tileSelectorScenes.clear();

    m_gameConfig = nullptr;
    m_gameConfigPath = "";

    m_currentLevel = nullptr;
    m_levelConfigList.clear();

    m_selectedLevelIndex = -1;
    m_selectedTilesetIndex = -1;
    m_selectedTileId = -1;

    m_tilesheetMaps.clear();
    m_tilestoreVector.clear();
    m_spawnInfoList.clear();
}

void MainWindowModel::newGameConfig(const std::string& filename, gsl::owner<LevelConfigT*> level) {
    loadGameConfig(filename);
    Ensures(m_gameConfig != nullptr);
    m_hasChanges = true;

    // Remove all existing levels
    for (auto it = m_gameConfig->getLevels().begin(); it != m_gameConfig->getLevels().end(); ++it) {
        m_gameConfig->removeLevel(it->second);
    }
    Ensures(m_gameConfig->getLevels().empty());

    m_gameConfig->addLevel(level);
}

void MainWindowModel::loadGameConfig(const std::string& filename) {
    GameConfigT* gameConfig(ctb::parser::parseGame(filename));
    Ensures(gameConfig != nullptr);
    m_gameConfig.reset(gameConfig);
    m_gameConfigPath = filename;
}

void MainWindowModel::saveGameConfig() {
    Expects(m_gameConfig != nullptr &&
            m_gameConfig->getLevels().size() == m_levelConfigList.size());

    // Save all tilestore information
    for (size_t levelIndex = 0; levelIndex < m_levelConfigList.size(); ++levelIndex) {
        LevelConfigT* level = m_levelConfigList.at(levelIndex);

        // For each tileset/layer
        for (size_t layerIndex = 0; layerIndex < level->getTilesets().size(); ++layerIndex) {
            TilesetConfigT& tileset = level->getTilesets().at(layerIndex);
            DynamicTilestoreT* tilestore =
                getLayerTilestore(static_cast<int>(levelIndex), static_cast<int>(layerIndex));
            tileset.setTileArrangement(tilestore->toTileString());
        }

        ctb::parser::saveLevel(level, m_gameConfigPath);
    }

    ctb::parser::saveGameLevels(m_gameConfigPath, m_gameConfig.get());
    m_hasChanges = false;
}

int MainWindowModel::addLevelConfig(LevelConfigT* level, bool setStatus) {
    if (setStatus) {
        m_hasChanges = true;
    }

    Expects(m_gameConfig != nullptr);

    // Create list of level configurations
    m_levelConfigList.push_back(level);
    int index = static_cast<int>(m_levelConfigList.size()) - 1;

    // Map all tilesheets (tile id --> bitmap)
    {
        QPixmapPtrT tilesetImg =
            std::make_unique<QPixmap>(QString::fromStdString(level->getTilesheetFilename()));

        // Create new tile id map
        m_tilesheetMaps.emplace_back();
        TileIdToPixmapMapT& tilesheetMap = m_tilesheetMaps.back();
        Ensures(m_levelConfigList.size() == m_tilesheetMaps.size());

        // Insert "transparent" texture with index 0 - empty tile
        QPixmap semiTransparentTexture(level->getTileWidth(), level->getTileHeight());
        semiTransparentTexture.fill(QColor(0, 0, 0, 35));
        tilesheetMap.insert({0, std::move(semiTransparentTexture)});

        const int maxTileId = level->getMaxTileId();
        Expects(maxTileId >= 0 && level->getTilesPerRow() > 0);

        for (int tileId = 0; tileId < maxTileId; ++tileId) {
            // Source rect in tilesheet
            QRect rect((level->getTileWidth() + level->getTileOffset()) *
                           (tileId % level->getTilesPerRow()),
                       (level->getTileHeight() + level->getTileOffset()) *
                           (tileId / level->getTilesPerRow()),
                       level->getTileWidth(), level->getTileHeight());

            // Map tile id to tile bitmap
            tilesheetMap.insert({tileId + 1, tilesetImg->copy(rect)});
        }
    }

    // Create tile selector scenes and tilestore from all layers.
    {
        auto tileSelector = std::make_unique<TileSelectorScene>(
            level, getTilesheetMap(index), [&](int tileId) { m_selectedTileId = tileId; });
        m_tileSelectorScenes.push_back(std::move(tileSelector));

        // Create tilestores from all layers
        m_tilestoreVector.emplace_back();
        TilesetTilestoreListT& stores = getLevelTilestores(index);
        for (auto& layer : level->getTilesets()) {
            DynamicTilestoreT store(layer.getTileArrangement(), level->getWidth(),
                                    level->getHeight(), level->getTileWidth(),
                                    level->getTileHeight());
            stores.push_back(std::move(store));
        }

        // Create spawn information
        SpawnInfoT spawnInfo(level->getWidth(), level->getHeight(), level->getTileWidth(),
                             level->getTileHeight());

        if (level->getFlagSpawn().isValid()) {
            spawnInfo.set(level->getFlagSpawn().x, level->getFlagSpawn().y,
                          fromSpawnType(SpawnType::kFlag));
        }
        if (level->getDoorL().isValid()) {
            spawnInfo.set(level->getDoorL().x, level->getDoorL().y,
                          fromSpawnType(SpawnType::kDoor));
        }
        if (level->getDoorR().isValid()) {
            spawnInfo.set(level->getDoorR().x, level->getDoorR().y,
                          fromSpawnType(SpawnType::kDoor));
        }

        for (size_t x = 0; x < level->getPlayerSpawns().size(); ++x) {
            auto& vec = level->getPlayerSpawns().at(x);
            for (auto y : vec) {
                spawnInfo.set(static_cast<int>(x), static_cast<int>(y),
                              fromSpawnType(SpawnType::kPlayer));
            }
        }

        for (auto& item : level->getBotSpawns()) {
            spawnInfo.set(item.getPosition().x, item.getPosition().y,
                          fromSpawnType(SpawnType::kBot));
        }

        for (auto& item : level->getWeaponSpawns()) {
            spawnInfo.set(item.getPosition().x, item.getPosition().y,
                          fromSpawnType(SpawnType::kBot));
        }
        m_spawnInfoList.push_back(std::move(spawnInfo));
    }

    // If thre is no level select select the first
    if (index == 0) {
        Ensures(m_selectedLevelIndex == -1);
        m_selectedLevelIndex = 0;
    }
    return index;
}

void MainWindowModel::removeLevelConfig() {
    m_hasChanges = true;

    Expects(m_gameConfig != nullptr);

    // Remove scenes
    m_tileSelectorScenes.erase(m_tileSelectorScenes.begin() + m_selectedLevelIndex);
    m_tilesetScenes.clear();

    // Remove tilestores and tilesheet map
    m_tilestoreVector.erase(std::next(m_tilestoreVector.begin(), m_selectedLevelIndex));
    m_tilesheetMaps.erase(std::next(m_tilesheetMaps.begin(), m_selectedLevelIndex));
    m_spawnInfoList.erase(std::next(m_spawnInfoList.begin(), m_selectedLevelIndex));

    // Remove LevelConfig and free memory
    m_levelConfigList.erase(m_levelConfigList.begin() + m_selectedLevelIndex);
    m_gameConfig->removeLevel(m_currentLevel);  // <-- frees memory
    m_currentLevel = nullptr;

    // Update level index
    m_selectedLevelIndex--;
    if (m_selectedLevelIndex == -1 && !m_levelConfigList.empty()) {
        m_selectedLevelIndex = 0;
    }
}

void MainWindowModel::resizeLevelConfig(int width, int height) {
    Expects(m_currentLevel != nullptr && m_selectedLevelIndex >= 0);
    m_hasChanges = true;

    // Purge old data
    m_tilesetScenes.clear();

    // Update level config
    m_currentLevel->setHeight(height);
    m_currentLevel->setWidth(width);

    // Resize tilestores
    auto& levelTilestores = getLevelTilestores(m_selectedLevelIndex);
    for (auto& tilestore : levelTilestores) {
        tilestore.setHeight(height);
        tilestore.setWidth(width);
    }

    // Resize spawn info
    getLevelSpawnInfo(m_selectedLevelIndex)->setHeight(height);
    getLevelSpawnInfo(m_selectedLevelIndex)->setWidth(width);
}

std::pair<TilesetConfigT, size_t> MainWindowModel::addNewTilesetConfig(int layerPosition,
                                                                       float friction,
                                                                       bool canCollideBots,
                                                                       bool canCollidePlayers,
                                                                       bool canCollideProjectiles) {
    size_t tileArrangementCount = static_cast<size_t>(m_currentLevel->getHeight()) *
                                  static_cast<size_t>(m_currentLevel->getWidth());
    if (static_cast<size_t>(std::numeric_limits<int>::max()) < tileArrangementCount) {
        throw std::invalid_argument("Invalid level size.");
    }

    // Allocate tileset with tile id 0
    std::string tileArrangement;
    tileArrangement.reserve(tileArrangementCount * 2);
    for (size_t i = 0; i < tileArrangementCount; ++i) {
        tileArrangement += "0 ";  // empty tile
    }

    // Remove last space ' '
    tileArrangement.pop_back();

    TilesetConfigT layer(layerPosition, friction, canCollideBots, canCollideProjectiles,
                         canCollidePlayers, std::move(tileArrangement));

    m_hasChanges = true;

    Expects(getLevelTilestores(m_selectedLevelIndex).size() ==
            m_currentLevel->getTilesets().size());

    // Create tilestore from layer
    TilesetTilestoreListT& stores = getLevelTilestores(m_selectedLevelIndex);
    DynamicTilestoreT store(layer.getTileArrangement(), m_currentLevel->getWidth(),
                            m_currentLevel->getHeight(), m_currentLevel->getTileWidth(),
                            m_currentLevel->getTileHeight());
    stores.push_back(std::move(store));

    // Add tileset/layer to level
    m_currentLevel->addTileset(layer);
    size_t index = m_currentLevel->getTilesets().size() - 1;

    // Create tileset scene and update UI
    setupTilesetConfig(index);
    return std::make_pair(layer, index);
}

void MainWindowModel::setupTilesetConfig(size_t index) {
    // 'push_back' protection
    Expects(m_tilesetScenes.size() == index);

    // Create TilesetScene
    DynamicTilestoreT* tilestore = getLayerTilestore(m_selectedLevelIndex, static_cast<int>(index));
    auto levelScene = std::make_unique<TilesetScene>(
        m_currentLevel, tilestore, getLevelSpawnInfo(m_selectedLevelIndex),
        getTilesheetMap(m_selectedLevelIndex),
        [&](TilesetItemCallbackAction action, TilesetScene::CoordinateT coordinate) {
            m_tilesetItemCallback(action, coordinate);
        });
    m_tilesetScenes.push_back(std::move(levelScene));

    // If thre is no tileset/layer select select the first
    if (index == 0) {
        m_selectedTilesetIndex = 0;
    }
}

void MainWindowModel::removeTilesetConfig() {
    if (m_selectedTilesetIndex == -1) {
        return;
    }

    m_hasChanges = true;

    m_tilesetScenes.erase(m_tilesetScenes.begin() + m_selectedTilesetIndex);

    TilesetTilestoreListT& stores = getLevelTilestores(m_selectedLevelIndex);
    stores.erase(std::next(stores.begin(), m_selectedTilesetIndex));

    // Free and remove tileset/layer from selected level
    m_currentLevel->getTilesets().erase(m_currentLevel->getTilesets().begin() +
                                        m_selectedTilesetIndex);

    // Update tileset/layer index
    m_selectedTilesetIndex--;
    if (m_selectedTilesetIndex == -1 && !m_currentLevel->getTilesets().empty()) {
        m_selectedTilesetIndex = 0;
    }
}

// ------------------------------------------------------------------------

gsl::not_null<TileIdToPixmapMapT*> MainWindowModel::getTilesheetMap(int index) {
    Expects(index >= 0 && static_cast<size_t>(index) < m_tilesheetMaps.size());
    auto it = std::next(m_tilesheetMaps.begin(), static_cast<ptrdiff_t>(index));
    Ensures(it != m_tilesheetMaps.end());
    return &(*it);
}

TilesetTilestoreListT& MainWindowModel::getLevelTilestores(int index) {
    Expects(index >= 0 && static_cast<size_t>(index) < m_tilestoreVector.size());
    auto it = std::next(m_tilestoreVector.begin(), static_cast<ptrdiff_t>(index));
    Ensures(it != m_tilestoreVector.end());
    return *it;
}

SpawnInfoT* MainWindowModel::getLevelSpawnInfo(int index) {
    Expects(index >= 0 && static_cast<size_t>(index) < m_spawnInfoList.size());
    auto it = std::next(m_spawnInfoList.begin(), static_cast<ptrdiff_t>(index));
    Ensures(it != m_spawnInfoList.end());
    return &(*it);
}

DynamicTilestoreT& MainWindowModel::getLayerTilestore(TilesetTilestoreListT& tilesetTilestores,
                                                      int index) {
    Expects(index >= 0 && static_cast<size_t>(index) < tilesetTilestores.size());
    auto it = std::next(tilesetTilestores.begin(), static_cast<ptrdiff_t>(index));
    Ensures(it != tilesetTilestores.end());
    return *it;
}

gsl::not_null<DynamicTilestoreT*> MainWindowModel::getLayerTilestore(int levelIndex,
                                                                     int layerIndex) {
    auto& tilestores = getLevelTilestores(levelIndex);
    return &getLayerTilestore(tilestores, layerIndex);
}

}  // namespace editor
}  // namespace ctb
