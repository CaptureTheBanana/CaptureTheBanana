// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <QCloseEvent>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QTranslator>
#include <iostream>

#include <boost/filesystem.hpp>
#include <common/ConsoleColor.hpp>
#include <common/Utils.hpp>
#include <parser/GameParser.hpp>
#include <parser/LevelParser.hpp>

#include "editor/gui/LayerSettingsDialog.hpp"
#include "editor/gui/LevelBackgroundSettingsDialog.hpp"
#include "editor/gui/LevelSettingsDialog.hpp"
#include "editor/gui/MainWindow.hpp"
#include "editor/gui/ResizeDialog.hpp"
#include "editor/gui/SetupDialog.hpp"
#include "editor/gui/TileDialog.hpp"
#include "editor/scene/GraphicsItems.hpp"
#include "editor/scene/GraphicsScenes.hpp"

namespace ctb {
namespace editor {
namespace {
using LevelTypeT = ctb::parser::LevelType;
using TypeSpawnT = ctb::parser::TypeSpawn;

/// \brief Converts a level type in a string.
///
/// \param type Level type
/// \return QString Converted string
QString getLevelType(LevelTypeT type) {
    switch (type) {
        case LevelTypeT::CENTER:
            return " (center)";
        case LevelTypeT::DEFAULT:
            return " (default)";
        case LevelTypeT::END:
            return " (end)";
        default:
            return "";
    }
}

/// \brief Returns the level name from the given level.
///
/// \param level Level
/// \return QString Level name (%filename% %level_type%)
QString getLevelName(gsl::not_null<LevelConfigT*> level) {
    // Extract filename without extenstion
    boost::filesystem::path p(level->getLevelFilename().c_str());
    QString filename = QString::fromStdString(p.stem().string());
    return filename + getLevelType(level->getLevelType());
}

/// \brief Returns the layer name from the given layer.
///
/// \param layer Layer/tileset
/// \return QString Layer/tileset name
QString getLayerName(TilesetConfigT& layer) {
    QString collisions = "[o,o,o]";
    if (layer.canCollideBots()) {
        collisions.replace(1, 1, 'x');
    }
    if (layer.canCollidePlayers()) {
        collisions.replace(3, 1, 'x');
    }
    if (layer.canCollideProjectiles()) {
        collisions.replace(5, 1, 'x');
    }
    return "Layer: " + QString::number(layer.getLayer()) + " " + collisions;
}
}  // namespace

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_ui(std::make_unique<Ui::MainWindow>()),
      m_model([&](TilesetItemCallbackAction a, TilesetScene::CoordinateT c) {
          tilesetItemCallback(a, c);
      }) {
    m_ui->setupUi(this);
    setWindowTitle("CaptureTheBanana++ Editor");
    createMenus();

    connect(m_ui->cbLevelId, SIGNAL(currentIndexChanged(int)), this,
            SLOT(cbLevelIdCurrentIndexChanged(int)));
    connect(m_ui->cbLayerId, SIGNAL(currentIndexChanged(int)), this,
            SLOT(cbLayerIdCurrentIndexChanged(int)));
}

void MainWindow::cbLevelIdCurrentIndexChanged(int index) {
    if (!m_active || m_disableEvents || getSelectedLevelIndex() == index) {
        return;
    }

    m_model.setSelectedLevelIndex(index);
    updateLevelUi();
}

void MainWindow::cbLayerIdCurrentIndexChanged(int index) {
    if (!m_active || m_disableEvents || getSelectedTilesetIndex() == index) {
        return;
    }

    m_model.setSelectedTilesetIndex(index);
    updateTilesetUi();
}

void MainWindow::showEvent(QShowEvent* /*event*/) {
    namespace cconsole = common::console;
    namespace cutils = common::utils;
    if (QCoreApplication::arguments().size() >= 2) {
        std::string path = QCoreApplication::arguments().at(1).toStdString();
        if (boost::filesystem::exists(path.c_str())) {
            // Load game and update UI
            loadGameConfig(cutils::toUnixPath(path));
            initGameUi();
        } else {
            std::cout << cconsole::yellow << "[WARNING] Game file does not exist!"
                      << cconsole::reset << std::endl;
        }
    }
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (m_active && hasChanges()) {
        QMessageBox::StandardButton reply = showQuitMessage();
        if (reply == QMessageBox::Save) {
            saveGame();
            event->accept();
        } else if (reply == QMessageBox::Cancel) {
            event->ignore();
        } else {
            event->accept();
        }
    }
}

// ----------------------------------------------------------------------------

void MainWindow::newGame() {
    if (m_active && hasChanges()) {
        QMessageBox::StandardButton reply = showQuitMessage();
        if (reply == QMessageBox::Save) {
            saveGame();
        } else if (reply == QMessageBox::Cancel) {
            return;
        }
    }

    SetupDialog dialog;
    dialog.setWindowTitle("Create new game");
    dialog.setModal(true);
    dialog.setGameTextBoxVisibility(true);
    int result = dialog.exec();
    if (result == QDialog::Accepted) {
        std::string gamePath = dialog.m_gameConfigPath;
        gsl::owner<LevelConfigT*> level = dialog.m_level.release();

        // Load game and add the level to the game. Finally update UI.
        newGameConfig(gamePath, level);
    }
}

void MainWindow::openGame() {
    if (m_active && hasChanges()) {
        QMessageBox::StandardButton reply = showQuitMessage();
        if (reply == QMessageBox::Save) {
            saveGame();
        } else if (reply == QMessageBox::Cancel) {
            return;
        }
    }

    // Open game file
    std::string filename =
        QFileDialog::getOpenFileName(this, "Open game file", ".", "Game file (game.xml)")
            .toStdString();
    if (filename.empty()) {
        return;
    }

    // Load game data and update UI.
    loadGameConfig(filename);
    initGameUi();
}

void MainWindow::saveGame() {
    if (!m_active) {
        return;
    }
    saveGameConfig();
}

void MainWindow::saveStartGame() {
    using namespace ctb::parser;
    if (!m_active) {
        return;
    }

    Expects(getGameConfig() != nullptr);

    saveGameConfig();

    GameValidatorResult result = validateGameConfig(getGameConfig());
    if (result.status != GameValidatorStatus::kOk) {
        QMessageBox::information(this, "Game validator", QString::fromStdString(to_string(result)),
                                 QMessageBox::Ok);
        return;
    }

    // Search for CaptureTheBanana application
    if (m_GameAppPath.empty()) {
        std::vector<std::string> paths = {"./CaptureTheBananal", "../ctb/CaptureTheBanana"};
        for (auto& item : paths) {
            QFileInfo fileInfo(QString::fromStdString(item));
            if (fileInfo.exists()) {
                m_GameAppPath = fileInfo.absoluteFilePath().toStdString();
                break;
            }
        }

        if (m_GameAppPath.empty()) {
            QString appName = "CaptureTheBanana";
#ifdef _WIN32
            appName.append(".exe");
#endif
            m_GameAppPath =
                QFileDialog::getOpenFileName(this, "Open game application", ".",
                                             "CaptureTheBanana++ application (" + appName + ")")
                    .toStdString();
            if (m_GameAppPath.empty()) {
                QMessageBox::information(this, "Information",
                                         "CaptureTheBanana++ application not found!",
                                         QMessageBox::Ok);
                return;
            }
        }
    }

    // Blocking system call
    std::string command = m_GameAppPath + " -ds " + m_model.getGameConfigPath();
    std::system(command.c_str());
}

void MainWindow::addLevel() {
    if (!m_active) {
        return;
    }

    SetupDialog dialog;
    dialog.setWindowTitle("Add new level");
    dialog.setModal(true);
    dialog.setGameTextBoxVisibility(false);
    dialog.m_gameConfigPath = m_model.getGameConfigPath();
    int result = dialog.exec();
    if (result == QDialog::Accepted) {
        // Check if name already exists
        for (auto& lvl : m_model.getGameConfig()->getLevels()) {
            if (dialog.m_level->getLevelFilename() == lvl.second->getLevelFilename()) {
                QMessageBox::information(
                    this, "Information",
                    QString::fromStdString("Level \"" + dialog.m_level->getLevelFilename() +
                                           "\" already exists."),
                    QMessageBox::Ok);
                return;
            }
        }

        gsl::owner<LevelConfigT*> level = dialog.m_level.release();

        // Add level to the GameConfig - takes ownership
        m_model.getGameConfig()->addLevel(level);
        addLevelConfig(level);
    }
}

void MainWindow::removeLevel() {
    if (!m_active || getSelectedLevelIndex() == -1) {
        return;
    }
    removeLevelConfig();
}

void MainWindow::resizeLevel() {
    if (!m_active || getSelectedLevelIndex() == -1) {
        return;
    }

    Expects(getCurrentLevel() != nullptr);

    ResizeDialog dialog;
    dialog.setWindowTitle("Resize layer");
    dialog.setModal(true);
    dialog.setHeight(getCurrentLevel()->getHeight());
    dialog.setWidth(getCurrentLevel()->getWidth());
    int result = dialog.exec();
    if (result == QDialog::Accepted) {
        resizeLevelConfig(dialog.getWidth(), dialog.getHeight());
    }
}

void MainWindow::levelBackgroundSettings() {
    namespace cutils = ctb::common::utils;
    if (!m_active || getSelectedLevelIndex() == -1) {
        return;
    }

    Expects(getCurrentLevel() != nullptr);

    LevelBackgroundSettingsDialog dialog(
        getCurrentLevel()->getBackgrounds(),
        cutils::getDirectoryPathWithSlash(m_model.getGameConfigPath()));
    dialog.setWindowTitle("Background settings");
    dialog.setModal(true);
    int result = dialog.exec();
    if (result == QDialog::Accepted) {
        // Update level background list
        getCurrentLevel()->getBackgrounds().clear();
        for (auto& item : dialog.getList()) {
            getCurrentLevel()->addBackground(item);
        }
        m_model.setChanged();
    }
}

void MainWindow::levelSettings() {
    if (!m_active || getSelectedLevelIndex() == -1) {
        return;
    }

    Expects(getCurrentLevel() != nullptr);

    LevelSettingsDialog dialog;
    dialog.setWindowTitle("Level settings");
    dialog.setModal(true);
    dialog.setGravitation(getCurrentLevel()->getGravitation());
    dialog.setLevelType(getCurrentLevel()->getLevelType());
    int result = dialog.exec();
    if (result == QDialog::Accepted) {
        getCurrentLevel()->setGravitation(dialog.getGravitation());
        getCurrentLevel()->setLevelType(dialog.getLevelType());

        // Update combobox text
        m_ui->cbLevelId->setItemText(getSelectedLevelIndex(), getLevelName(getCurrentLevel()));
        m_model.setChanged();
    }
}

void MainWindow::addLayer() {
    if (!m_active) {
        return;
    }

    LayerSettingsDialog dialog;
    dialog.setWindowTitle("Add layer");
    dialog.setModal(true);
    dialog.setLayerPosition(5);
    dialog.setFriction(0.8f);
    dialog.setCollideBots(false);
    dialog.setCollidePlayers(false);
    dialog.setCollideProjectiles(false);
    int result = dialog.exec();
    if (result == QDialog::Accepted) {
        addNewTilesetConfig(dialog.getLayerPosition(), dialog.getFriction(),
                            dialog.canCollideBots(), dialog.canCollidePlayers(),
                            dialog.canCollideProjectiles());
    }
}

void MainWindow::removeLayer() {
    if (!m_active || getSelectedTilesetIndex() == -1) {
        return;
    }
    removeTilesetConfig();
}

void MainWindow::layerSettings() {
    if (!m_active || getSelectedTilesetIndex() == -1) {
        return;
    }

    Expects(getCurrentLevel() != nullptr);

    TilesetConfigT& layer =
        getCurrentLevel()->getTilesets().at(static_cast<size_t>(getSelectedTilesetIndex()));

    LayerSettingsDialog dialog;
    dialog.setWindowTitle("Layer settings");
    dialog.setModal(true);
    dialog.setLayerPosition(layer.getLayer());
    dialog.setFriction(layer.getFriction());
    dialog.setCollideBots(layer.canCollideBots());
    dialog.setCollidePlayers(layer.canCollidePlayers());
    dialog.setCollideProjectiles(layer.canCollideProjectiles());
    int result = dialog.exec();
    if (result == QDialog::Accepted) {
        layer.setLayer(dialog.getLayerPosition());
        layer.setFriction(dialog.getFriction());
        layer.setCollideBots(dialog.canCollideBots());
        layer.setCollidePlayers(dialog.canCollidePlayers());
        layer.setCollideProjectiles(dialog.canCollideProjectiles());

        // Update combobox text
        m_ui->cbLayerId->setItemText(getSelectedTilesetIndex(), getLayerName(layer));
        m_model.setChanged();
    }
}

void MainWindow::validateGame() {
    using namespace ctb::parser;
    if (!m_active) {
        return;
    }

    Expects(getGameConfig() != nullptr);

    GameValidatorResult result = validateGameConfig(getGameConfig());
    QMessageBox::information(this, "Game validator", QString::fromStdString(to_string(result)),
                             QMessageBox::Ok);
}

void MainWindow::about() {
    QMessageBox::about(this, "About",
                       "CaptureTheBanana++ Editor\n\nMIT licensed; visit "
                       "https://opensource.org/licenses/MIT for details.\nCopyright (c) 2018 the "
                       "CaptureTheBanana++ contributors (see CONTRIBUTORS.md)");
}

void MainWindow::createMenus() {
    // File menu
    m_newAct = std::make_unique<QAction>("&New...", this);
    m_newAct->setShortcuts(QKeySequence::New);
    connect(m_newAct.get(), &QAction::triggered, this, &MainWindow::newGame);

    m_openAct = std::make_unique<QAction>("&Open...", this);
    m_openAct->setShortcuts(QKeySequence::Open);
    connect(m_openAct.get(), &QAction::triggered, this, &MainWindow::openGame);

    m_saveAct = std::make_unique<QAction>("&Save all", this);
    m_saveAct->setShortcuts(QKeySequence::Save);
    connect(m_saveAct.get(), &QAction::triggered, this, &MainWindow::saveGame);

    m_saveStartAct = std::make_unique<QAction>("&Save all and start game", this);
    m_saveStartAct->setShortcuts(QKeySequence::Save);
    connect(m_saveStartAct.get(), &QAction::triggered, this, &MainWindow::saveStartGame);

    m_exitAct = std::make_unique<QAction>("E&xit", this);
    m_exitAct->setShortcuts(QKeySequence::Quit);
    connect(m_exitAct.get(), &QAction::triggered, this, &QWidget::close);

    // Edit menu
    m_addLeveltAct = std::make_unique<QAction>("New level...", this);
    connect(m_addLeveltAct.get(), &QAction::triggered, this, &MainWindow::addLevel);

    m_removeLevelAct = std::make_unique<QAction>("Remove current level", this);
    connect(m_removeLevelAct.get(), &QAction::triggered, this, &MainWindow::removeLevel);

    m_resizeLevelAct = std::make_unique<QAction>("Resize level", this);
    connect(m_resizeLevelAct.get(), &QAction::triggered, this, &MainWindow::resizeLevel);

    m_levelBackgroundSettingsAct =
        std::make_unique<QAction>("Edit background settings of current level");
    connect(m_levelBackgroundSettingsAct.get(), &QAction::triggered, this,
            &MainWindow::levelBackgroundSettings);

    m_levelSettingsAct = std::make_unique<QAction>("Edit settings of current level");
    connect(m_levelSettingsAct.get(), &QAction::triggered, this, &MainWindow::levelSettings);

    m_addLayerAct = std::make_unique<QAction>("New layer");
    connect(m_addLayerAct.get(), &QAction::triggered, this, &MainWindow::addLayer);

    m_removeLayerAct = std::make_unique<QAction>("Remove current layer");
    connect(m_removeLayerAct.get(), &QAction::triggered, this, &MainWindow::removeLayer);

    m_layerSettingsAct = std::make_unique<QAction>("Edit settings of current layer");
    connect(m_layerSettingsAct.get(), &QAction::triggered, this, &MainWindow::layerSettings);

    m_validateGameAct = std::make_unique<QAction>("Validate configuration", this);
    connect(m_validateGameAct.get(), &QAction::triggered, this, &MainWindow::validateGame);

    // Help menu
    m_aboutAct = std::make_unique<QAction>("&About", this);
    connect(m_aboutAct.get(), &QAction::triggered, this, &MainWindow::about);

    m_aboutQtAct = std::make_unique<QAction>("About &Qt", this);
    connect(m_aboutQtAct.get(), &QAction::triggered, qApp, &QApplication::aboutQt);

    m_fileMenu.reset(menuBar()->addMenu("&File"));
    m_fileMenu->addAction(m_newAct.get());
    m_fileMenu->addAction(m_openAct.get());
    m_fileMenu->addAction(m_saveAct.get());
    m_fileMenu->addAction(m_saveStartAct.get());
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAct.get());

    m_editMenu.reset(menuBar()->addMenu("&Edit"));

    m_levelSubMenu.reset(m_editMenu->addMenu("&Level"));
    m_levelSubMenu->addAction(m_addLeveltAct.get());
    m_levelSubMenu->addAction(m_removeLevelAct.get());
    m_levelSubMenu->addSeparator();
    m_levelSubMenu->addAction(m_resizeLevelAct.get());
    m_levelSubMenu->addAction(m_levelBackgroundSettingsAct.get());
    m_levelSubMenu->addAction(m_levelSettingsAct.get());

    m_layerSubMenu.reset(m_editMenu->addMenu("&Layer"));
    m_layerSubMenu->addAction(m_addLayerAct.get());
    m_layerSubMenu->addAction(m_removeLayerAct.get());
    m_layerSubMenu->addSeparator();
    m_layerSubMenu->addAction(m_layerSettingsAct.get());

    m_editMenu->addSeparator();
    m_editMenu->addAction(m_validateGameAct.get());

    m_helpMenu.reset(menuBar()->addMenu("&Help"));
    m_helpMenu->addAction(m_aboutAct.get());
    m_helpMenu->addAction(m_aboutQtAct.get());
}

void MainWindow::initGameUi() {
    m_active = true;

    // Load all level configurations and prepair backend
    for (auto& lvl : getGameConfig()->getLevels()) {
        addLevelConfig(lvl.second, false);
    }
}

void MainWindow::resetGameUi() {
    m_active = false;
    m_disableEvents = false;

    m_ui->gvLayer->setScene(nullptr);
    m_ui->gvTileSelector->setScene(nullptr);
    m_ui->cbLevelId->clear();
    m_ui->cbLayerId->clear();

    m_model.reset();
}

void MainWindow::updateLevelUi() {
    if (m_disableEvents) {
        return;
    }

    Expects(m_ui->cbLevelId->currentIndex() == getSelectedLevelIndex());
    if (getSelectedLevelIndex() < 0) {
        m_model.setSelectedLevelIndex(-1);
        m_model.setCurrentLevel(-1);
        m_ui->gvTileSelector->setScene(nullptr);
        m_ui->cbLevelId->clear();

        // Reset tileset/layer
        m_model.setSelectedTilesetIndex(-1);
        m_ui->gvLayer->setScene(nullptr);
        m_ui->cbLayerId->clear();
        return;
    }

    // Temporary disable all events
    m_disableEvents = true;

    m_model.setCurrentLevel(getSelectedLevelIndex());

    m_model.setSelectedTilesetIndex(-1);
    m_ui->cbLayerId->clear();
    m_model.getTilesetScenes().clear();

    // -------------------

    for (size_t i = 0; i < getCurrentLevel()->getTilesets().size(); ++i) {
        setupTilesetConfig(i);
    }

    m_ui->gvTileSelector->setScene(
        m_model.getTileSelectorScenes().at(static_cast<size_t>(getSelectedLevelIndex())).get());
}

void MainWindow::updateTilesetUi() {
    if (m_disableEvents) {
        return;
    }

    Expects(m_ui->cbLayerId->currentIndex() == getSelectedTilesetIndex());
    if (getSelectedTilesetIndex() < 0) {
        m_model.setSelectedTilesetIndex(-1);
        m_ui->gvLayer->setScene(nullptr);
        m_ui->cbLayerId->clear();
        return;
    }
    m_ui->gvLayer->setScene(
        m_model.getTilesetScenes().at(static_cast<size_t>(getSelectedTilesetIndex())).get());
}

QMessageBox::StandardButton MainWindow::showQuitMessage() {
    return QMessageBox::question(this, "Game configuration has been modified",
                                 tr("<b>Do you want to save changes?</b><br><br>Your changes will "
                                    "be lost if you close this game configuration without saving."),
                                 QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                 QMessageBox::Save);
}

void MainWindow::tilesetItemCallback(TilesetItemCallbackAction action,
                                     TilesetScene::CoordinateT coordinate) {
    if (!m_active || getSelectedLevelIndex() == -1 ||
        (action == TilesetItemCallbackAction::kMarkTile && getSelectedTileId() == -1)) {
        return;
    }

    switch (action) {
        case TilesetItemCallbackAction::kDetails: {
            // Open tile configuration dialog and save changes
            Expects(getCurrentLevel() != nullptr);

            bool isDoorL = getCurrentLevel()->getDoorL().x == coordinate.first &&
                           getCurrentLevel()->getDoorL().y == coordinate.second;
            bool isDoorR = getCurrentLevel()->getDoorR().x == coordinate.first &&
                           getCurrentLevel()->getDoorR().y == coordinate.second;
            bool isFlagsSpawn = getCurrentLevel()->getFlagSpawn().x == coordinate.first &&
                                getCurrentLevel()->getFlagSpawn().y == coordinate.second;

            auto& spawns =
                getCurrentLevel()->getPlayerSpawns().at(static_cast<size_t>(coordinate.first));
            bool isPlayerSpawn =
                std::find(spawns.begin(), spawns.end(), coordinate.second) != spawns.end();

            auto isInVectorFn = [&](int x, int y, std::vector<TypeSpawnT>& vec) -> int {
                int index = -1;
                for (size_t i = 0; i < vec.size(); ++i) {
                    if (vec[i].getPosition().x == x && vec[i].getPosition().y == y) {
                        index = static_cast<int>(i);
                        break;
                    }
                }
                return index;
            };

            int botSpawnIndex = isInVectorFn(coordinate.first, coordinate.second,
                                             getCurrentLevel()->getBotSpawns());
            int weaponSpawnIndex = isInVectorFn(coordinate.first, coordinate.second,
                                                getCurrentLevel()->getWeaponSpawns());

            TileDialog dialog;
            dialog.setWindowTitle("Tile configuration (x: " + QString::number(coordinate.first) +
                                  ", y: " + QString::number(coordinate.second) + ")");
            dialog.setModal(true);
            dialog.addBots(getGameConfig()->getBots());
            dialog.addWeapons(getGameConfig()->getWeapons());
            dialog.setDoorL(isDoorL);
            dialog.setDoorR(isDoorR);
            dialog.setFlagSpawn(isFlagsSpawn);
            dialog.setPlayerSpawn(isPlayerSpawn);
            dialog.setBotSpawn(botSpawnIndex != -1);
            dialog.setWeaponSpawn(weaponSpawnIndex != -1);

            // Get bot name
            if (botSpawnIndex != -1) {
                dialog.setBotName(getCurrentLevel()
                                      ->getBotSpawns()
                                      .at(static_cast<size_t>(botSpawnIndex))
                                      .getType());
            }

            // Get weapon name
            if (weaponSpawnIndex != -1) {
                dialog.setWeaponName(getCurrentLevel()
                                         ->getWeaponSpawns()
                                         .at(static_cast<size_t>(weaponSpawnIndex))
                                         .getType());
            }

            int result = dialog.exec();
            if (result == QDialog::Accepted) {
                m_model.setChanged();
                SpawnType spawnType =
                    toSpawnType(m_model.getLevelSpawnInfo(getSelectedLevelIndex())
                                    ->get(coordinate.first, coordinate.second));
                bool isMarker = hasSpawnType(spawnType, SpawnType::kMark);

                // Update left door
                if (!isDoorL && dialog.isDoorL()) {
                    // Reset old door
                    if (getCurrentLevel()->getDoorL().isValid()) {
                        m_model.getLevelSpawnInfo(getSelectedLevelIndex())
                            ->set(getCurrentLevel()->getDoorL().x, getCurrentLevel()->getDoorL().y,
                                  fromSpawnType(SpawnType::kNone));
                    }

                    // Update
                    getCurrentLevel()->getDoorL().x = coordinate.first;
                    getCurrentLevel()->getDoorL().y = coordinate.second;
                    spawnType = SpawnType::kDoor;
                } else if (isDoorL && !dialog.isDoorL()) {
                    getCurrentLevel()->getDoorL().x = -1;
                    getCurrentLevel()->getDoorL().y = -1;
                    spawnType = SpawnType::kNone;
                }

                // Update right door
                if (!isDoorR && dialog.isDoorR()) {
                    // Reset old door
                    if (getCurrentLevel()->getDoorR().isValid()) {
                        m_model.getLevelSpawnInfo(getSelectedLevelIndex())
                            ->set(getCurrentLevel()->getDoorR().x, getCurrentLevel()->getDoorR().y,
                                  fromSpawnType(SpawnType::kNone));
                    }

                    // Update
                    getCurrentLevel()->getDoorR().x = coordinate.first;
                    getCurrentLevel()->getDoorR().y = coordinate.second;
                    spawnType = SpawnType::kDoor;
                } else if (isDoorR && !dialog.isDoorR()) {
                    getCurrentLevel()->getDoorR().x = -1;
                    getCurrentLevel()->getDoorR().y = -1;
                    spawnType = SpawnType::kNone;
                }

                // Update flag spawn
                if (!isFlagsSpawn && dialog.isFlagSpawn()) {
                    // Reset old flag point
                    if (getCurrentLevel()->getFlagSpawn().isValid()) {
                        m_model.getLevelSpawnInfo(getSelectedLevelIndex())
                            ->set(getCurrentLevel()->getFlagSpawn().x,
                                  getCurrentLevel()->getFlagSpawn().y,
                                  fromSpawnType(SpawnType::kNone));
                    }

                    // Update
                    getCurrentLevel()->getFlagSpawn().x = coordinate.first;
                    getCurrentLevel()->getFlagSpawn().y = coordinate.second;
                    spawnType = SpawnType::kFlag;
                } else if (isFlagsSpawn && !dialog.isFlagSpawn()) {
                    getCurrentLevel()->getFlagSpawn().x = -1;
                    getCurrentLevel()->getFlagSpawn().y = -1;
                    spawnType = SpawnType::kNone;
                }

                // Update player spawn
                if (!isPlayerSpawn && dialog.isPlayerSpawn()) {
                    getCurrentLevel()->setPlayerSpawn(coordinate.first, coordinate.second);
                    spawnType = SpawnType::kPlayer;
                } else if (isPlayerSpawn && !dialog.isPlayerSpawn()) {
                    getCurrentLevel()->removePlayerSpawn(coordinate.first, coordinate.second);
                    spawnType = SpawnType::kNone;
                }

                // Update bot spawn
                if (dialog.isBotSpawn() && botSpawnIndex == -1) {
                    getCurrentLevel()->getBotSpawns().emplace_back(
                        coordinate.first, coordinate.second, dialog.getBotName());
                    spawnType = SpawnType::kBot;
                } else if (!dialog.isBotSpawn() && botSpawnIndex >= 0) {
                    getCurrentLevel()->getBotSpawns().erase(
                        getCurrentLevel()->getBotSpawns().begin() + botSpawnIndex);
                    spawnType = SpawnType::kNone;
                }

                // Update weapon spawn
                if (dialog.isWeaponSpawn() && weaponSpawnIndex == -1) {
                    getCurrentLevel()->getWeaponSpawns().emplace_back(
                        coordinate.first, coordinate.second, dialog.getWeaponName());
                    spawnType = SpawnType::kWeapon;
                } else if (!dialog.isWeaponSpawn() && weaponSpawnIndex >= 0) {
                    getCurrentLevel()->getWeaponSpawns().erase(
                        getCurrentLevel()->getWeaponSpawns().begin() + weaponSpawnIndex);
                    spawnType = SpawnType::kNone;
                }

                if (isMarker) {
                    setSpawnType(spawnType, SpawnType::kMark);
                }
                m_model.getLevelSpawnInfo(getSelectedLevelIndex())
                    ->set(coordinate.first, coordinate.second, fromSpawnType(spawnType));

                // Update UI
                m_model.getTilesetScenes()
                    .at(static_cast<size_t>(getSelectedTilesetIndex()))
                    ->update();
            }
            break;
        }
        case TilesetItemCallbackAction::kMarkTile: {
            // Mark current layer tile with the selected tile id.
            m_model.setChanged();
            DynamicTilestoreT* tilestore =
                m_model.getLayerTilestore(getSelectedLevelIndex(), getSelectedTilesetIndex());
            tilestore->set(coordinate.first, coordinate.second, getSelectedTileId());

            // Update UI
            m_model.getTilesetScenes().at(static_cast<size_t>(getSelectedTilesetIndex()))->update();
            break;
        }
        case TilesetItemCallbackAction::kRemoveTile: {
            // Set current layer tile id to 0 (empty).
            m_model.setChanged();
            DynamicTilestoreT* tilestore =
                m_model.getLayerTilestore(getSelectedLevelIndex(), getSelectedTilesetIndex());
            tilestore->set(coordinate.first, coordinate.second, 0 /*no tile*/);

            // Update UI
            m_model.getTilesetScenes().at(static_cast<size_t>(getSelectedTilesetIndex()))->update();
            break;
        }
        case TilesetItemCallbackAction::kSetSpawn: {
            // Set player spawnpoint
            m_model.setChanged();
            getCurrentLevel()->setPlayerSpawn(coordinate.first, coordinate.second);

            SpawnType spawnType = toSpawnType(m_model.getLevelSpawnInfo(getSelectedLevelIndex())
                                                  ->get(coordinate.first, coordinate.second));
            if (hasSpawnType(spawnType, SpawnType::kMark)) {
                spawnType = SpawnType::kPlayer;
                setSpawnType(spawnType, SpawnType::kMark);
            } else {
                spawnType = SpawnType::kPlayer;
            }

            m_model.getLevelSpawnInfo(getSelectedLevelIndex())
                ->set(coordinate.first, coordinate.second, fromSpawnType(spawnType));

            // Update UI
            m_model.getTilesetScenes().at(static_cast<size_t>(getSelectedTilesetIndex()))->update();
            break;
        }
        default:
            break;
    }
}

// ----------------------------------------------------------------------------

void MainWindow::newGameConfig(const std::string& filename, gsl::owner<LevelConfigT*> level) {
    // Reset game data
    resetGameUi();
    m_model.newGameConfig(filename, level);
    initGameUi();
}

void MainWindow::loadGameConfig(const std::string& filename) {
    // Reset game data
    resetGameUi();
    m_model.loadGameConfig(filename);
}

void MainWindow::saveGameConfig() {
    m_model.saveGameConfig();
}

void MainWindow::addLevelConfig(LevelConfigT* level, bool setStatus) {
    // Temporary disable all events
    m_disableEvents = true;

    // Add level config
    int index = m_model.addLevelConfig(level, setStatus);

    // Fill level combobox
    m_ui->cbLevelId->addItem(getLevelName(level));

    // Enable events
    m_disableEvents = false;

    // If thre is no level select select the first
    if (index == 0) {
        Expects(m_model.getSelectedLevelIndex() == 0);
        m_ui->cbLevelId->setCurrentIndex(0);
        updateLevelUi();
    }
}

void MainWindow::removeLevelConfig() {
    // Temporary disable all events
    m_disableEvents = true;
    m_model.setChanged();

    Expects(m_active && getGameConfig() != nullptr);

    // Reset level UI
    m_ui->gvTileSelector->setScene(nullptr);
    m_ui->cbLevelId->removeItem(getSelectedLevelIndex());

    // Reset tileset/layer UI
    m_ui->gvLayer->setScene(nullptr);
    m_ui->cbLayerId->clear();

    // Remove level config
    m_model.removeLevelConfig();

    m_ui->cbLevelId->setCurrentIndex(m_model.getSelectedLevelIndex());

    // Enable events
    m_disableEvents = false;

    updateLevelUi();
}

void MainWindow::resizeLevelConfig(int width, int height) {
    if (getSelectedLevelIndex() == -1) {
        return;
    }

    Expects(getCurrentLevel() != nullptr);

    // Temporary disable all events
    m_disableEvents = true;
    m_model.setChanged();

    // Clear layer scene
    m_ui->gvLayer->setScene(nullptr);

    // Resize level
    m_model.resizeLevelConfig(width, height);

    m_disableEvents = false;

    // Reload level
    updateLevelUi();
}

void MainWindow::addNewTilesetConfig(int layerPosition,
                                     float friction,
                                     bool canCollideBots,
                                     bool canCollidePlayers,
                                     bool canCollideProjectiles) {
    auto layer = m_model.addNewTilesetConfig(layerPosition, friction, canCollideBots,
                                             canCollidePlayers, canCollideProjectiles);

    // Fill combobox
    m_ui->cbLayerId->addItem(getLayerName(layer.first));

    // Enable events
    m_disableEvents = false;

    // If thre is no tileset/layer select select the first
    if (layer.second == 0) {
        Expects(m_model.getSelectedTilesetIndex() == 0);
        m_ui->cbLayerId->setCurrentIndex(0);
        updateTilesetUi();
    }
}

void MainWindow::setupTilesetConfig(size_t index) {
    // Temporary disable all events
    m_disableEvents = true;

    // Create tilestore and TilesetScene
    m_model.setupTilesetConfig(index);

    // Fill combobox
    m_ui->cbLayerId->addItem(getLayerName(m_model.getLayerFromSelectedLevel(index)));

    // Enable events
    m_disableEvents = false;

    // If thre is no tileset/layer select select the first
    if (index == 0) {
        Expects(m_model.getSelectedTilesetIndex() == 0);
        m_ui->cbLayerId->setCurrentIndex(0);
        Expects(m_ui->cbLayerId->currentIndex() == 0);
        updateTilesetUi();
    }
}

void MainWindow::removeTilesetConfig() {
    if (getSelectedTilesetIndex() == -1) {
        return;
    }

    // Temporary disable all events
    m_disableEvents = true;

    m_ui->gvLayer->setScene(nullptr);
    m_ui->cbLayerId->removeItem(getSelectedTilesetIndex());

    // Remove selected tileset
    m_model.removeTilesetConfig();

    m_ui->cbLayerId->setCurrentIndex(getSelectedTilesetIndex());

    // Enable events
    m_disableEvents = false;

    updateTilesetUi();
}

}  // namespace editor
}  // namespace ctb
