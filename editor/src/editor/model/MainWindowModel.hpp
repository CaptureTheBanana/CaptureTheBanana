// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef MAINWINDOWMODEL_HPP
#define MAINWINDOWMODEL_HPP

#include <algorithm>  // std::find
#include <functional>
#include <iterator>  // std::next
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>  // std::hash
#include <vector>

#include <parser/DynamicTilestore.hpp>
#include <parser/GameConfig.hpp>
#include <parser/LevelConfig.hpp>
#include <parser/TilesetConfig.hpp>

#include "editor/SpawnInfo.hpp"
#include "editor/scene/GraphicsItems.hpp"
#include "editor/scene/GraphicsScenes.hpp"

#include <gsl/gsl>

namespace ctb {
namespace editor {

using GameConfigT = ctb::parser::GameConfig;
using GameConfigPtrT = std::unique_ptr<GameConfigT>;
using TilesetConfigT = ctb::parser::TilesetConfig;
using LevelConfigT = ctb::parser::LevelConfig;
/// Contains the game level list
using LevelConfigPtrVectorT = std::vector<LevelConfigT*>;

using TilesetScenePtrT = std::unique_ptr<TilesetScene>;
using TilesetScenePtrVectorT = std::vector<TilesetScenePtrT>;
using TileSelectorScenePtrT = std::unique_ptr<TileSelectorScene>;
using TileSelectorScenePtrVectorT = std::vector<TileSelectorScenePtrT>;

using QPixmapPtrT = std::unique_ptr<QPixmap>;

using DynamicTilestoreT = ctb::DynamicTilestore;

/// Tile id to pixmap map
using TileIdToPixmapMapT = std::unordered_map<int /*tile id*/, QPixmap>;
/// List of all tile id maps from selected level
using TileIdToPixmapMapListT = std::list<TileIdToPixmapMapT>;  // see note below

/// Contains all tilesets within a level
using TilesetTilestoreListT = std::list<DynamicTilestoreT>;  // see note below
/// List of all levels with a inner tileset list of tilestores.
using LevelTilestoreListT = std::list<TilesetTilestoreListT>;  // see note below

/// List of level spawn information
using LevelSpawnInfoT = std::list<SpawnInfoT>;  // see note below

// Note: We have to use a std::list to ensure a fixed memory address to the object.

class MainWindowModel {
   public:
    explicit MainWindowModel(GraphicsTilesetItem::CallbackT&& tilesetCallback);
    virtual ~MainWindowModel() = default;

    void reset();

    /// \brief Loads the given game and adds a level.
    ///
    /// \param filename Path to the game.xml
    /// \param level The level to add
    void newGameConfig(const std::string& filename, gsl::owner<LevelConfigT*> level);

    /// \brief Loads the given game.
    ///
    /// \param filename Path to the game.xml
    void loadGameConfig(const std::string& filename);

    /// Saves the selected game.
    void saveGameConfig();

    /// \brief Adds the given level to the game.
    ///
    /// \param level The level to add
    /// \param setStatus Sets hasChanged status to true
    /// \return int Index from the added level
    int addLevelConfig(LevelConfigT* level, bool setStatus = true);

    /// Removes the selected level from the game.
    void removeLevelConfig();

    /// \brief Resize the selected level.
    ///
    /// \param width New width
    /// \param height New height
    void resizeLevelConfig(int width, int height);

    /// \brief Adds a new tileset configuration (layer) to the selected level.
    ///
    /// \param layerPosition Layer position
    /// \param friction Friction value
    /// \param canCollideBots true if bots collide with tiles of this layer.
    /// \param canCollidePlayers true if player collide with tiles of this layer.
    /// \param canCollideProjectiles true if projectiles collide with tiles of this layer.
    /// \return std::pair<TilesetConfigT, int> Returns pointer to layer/tileset and index.
    std::pair<TilesetConfigT, size_t> addNewTilesetConfig(int layerPosition,
                                                          float friction,
                                                          bool canCollideBots,
                                                          bool canCollidePlayers,
                                                          bool canCollideProjectiles);

    /// \brief Adds the given tileset configuration (layer) to the selected level.
    ///
    /// \param index Tileset index
    void setupTilesetConfig(size_t index);

    /// Removes the selected tileset configuration (layer) from the leve.
    void removeTilesetConfig();

    // ------------------------------------------------------------------------

    /// \brief Returns a tile id to pixmap map from the given tilesheet index.
    ///
    /// \param index Tilesheet index
    /// \return gsl::not_null<MainWindow::TileIdToPixmapMapT*> Returns a tile id to pixmap map
    gsl::not_null<TileIdToPixmapMapT*> getTilesheetMap(int index);

    /// \brief Returns a layer/tileset list from the given layer/tileset index.
    ///
    /// \param index Layer/tileset index
    /// \return TilesetTilestoreListT
    TilesetTilestoreListT& getLevelTilestores(int index);

    /// \brief Returns a tilestore with spawn infromation from the given level index.
    ///
    /// \param index Level index
    /// \return SpawnInfoT Tilestore with spawn infromation
    SpawnInfoT* getLevelSpawnInfo(int index);

    /// \brief Returns a tilestore from the given tilestore list and index.
    ///
    /// \param tilesetTilestores Tilestore list
    /// \param index Index
    /// \return DynamicTilestoreT Tilestore
    DynamicTilestoreT& getLayerTilestore(TilesetTilestoreListT& tilesetTilestores, int index);

    /// \brief Returns a tilestore from the given level and layer index.
    ///
    /// \param levelIndex Level index
    /// \param layerIndex Layer/tileset index
    /// \return gsl::not_null<DynamicTilestoreT*> Tilestore
    gsl::not_null<DynamicTilestoreT*> getLayerTilestore(int levelIndex, int layerIndex);

    // ------------------------------------------------------------------------

    /// Indicates whether there are changes
    bool hasChanges() { return m_hasChanges; }
    /// Set changed indicatior to true.
    void setChanged() { m_hasChanges = true; }

    /// Returns selected level index or -1.
    int getSelectedLevelIndex() { return m_selectedLevelIndex; }

    /// \brief Sets selected level index
    ///
    /// \param index Index
    void setSelectedLevelIndex(int index) {
        Expects(index == -1 || (index >= 0 && index < static_cast<int>(m_levelConfigList.size())));
        m_selectedLevelIndex = index;
    }

    /// Returns selected layer/tileset index or -1.
    int getSelectedTilesetIndex() { return m_selectedTilesetIndex; }

    /// \brief Sets selected layer/tileset index
    ///
    /// \param index Index
    void setSelectedTilesetIndex(int index) {
        Expects(index == -1 ||
                (index >= 0 && index < static_cast<int>(m_currentLevel->getTilesets().size())));
        m_selectedTilesetIndex = index;
    }

    /// Returns selected tile id or -1.
    int getSelectedTileId() { return m_selectedTileId; }

    /// \brief Get layer/tileset by index from selected level.
    ///
    /// \param index Index
    /// \return TilesetConfigT layer/tileset
    TilesetConfigT& getLayerFromSelectedLevel(size_t index) {
        return m_currentLevel->getTilesets().at(index);
    }

    /// \brief Returns the path to the game config
    ///
    /// \return std::string Path to the game config
    std::string getGameConfigPath() { return m_gameConfigPath; }

    /// \brief Returns game config pointer
    ///
    /// \return GameConfigPtrT game config pointer
    GameConfigT* getGameConfig() { return m_gameConfig.get(); }

    /// \brief Return current level config
    ///
    /// \return LevelConfigPtrVectorT selected level config
    LevelConfigT* getCurrentLevel() { return m_currentLevel; }

    /// \brief Sets the current level or -1 to null.
    ///
    /// \param index Index
    void setCurrentLevel(int index) {
        if (index == -1) {
            m_currentLevel = nullptr;
        } else {
            Expects(index >= 0 && index < static_cast<int>(m_levelConfigList.size()));
            m_currentLevel = m_levelConfigList.at(static_cast<size_t>(index));
        }
    }

    /// Returns tile selector scenes
    TileSelectorScenePtrVectorT& getTileSelectorScenes() { return m_tileSelectorScenes; }

    /// Returns layer/tileset scenes
    TilesetScenePtrVectorT& getTilesetScenes() { return m_tilesetScenes; }

   private:
    /// Indicates whether there are changes
    bool m_hasChanges{false};

    /// Game configuration pointer - owner
    GameConfigPtrT m_gameConfig{nullptr};
    /// Game configuration path
    std::string m_gameConfigPath;
    /// Level configuration pointer list - non-owning
    LevelConfigPtrVectorT m_levelConfigList{};

    /// Level tileset/layer list from selected level
    TilesetScenePtrVectorT m_tilesetScenes{};
    /// Tileset selector scene list
    TileSelectorScenePtrVectorT m_tileSelectorScenes{};

    /// Pointer to the current level - non-owning
    LevelConfigT* m_currentLevel{nullptr};

    /// Current level (scene) index
    int m_selectedLevelIndex{-1};
    /// Current tileset/layer index
    int m_selectedTilesetIndex{-1};
    /// Selected tileset id
    int m_selectedTileId{-1};

    /// List of tileset id to QPixmap maps
    TileIdToPixmapMapListT m_tilesheetMaps{};

    /// Current representation
    LevelTilestoreListT m_tilestoreVector{};

    /// List of spawn information
    LevelSpawnInfoT m_spawnInfoList{};

    /// Tileset item callback
    GraphicsTilesetItem::CallbackT m_tilesetItemCallback;
};

}  // namespace editor
}  // namespace ctb

#endif  // MAINWINDOWMODEL_HPP
