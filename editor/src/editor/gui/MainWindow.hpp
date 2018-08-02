// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef _MAINWINDOW_HPP
#define _MAINWINDOW_HPP

#include <QMainWindow>
#include <QMessageBox>
#include <QPixmap>
#include <functional>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <gsl/gsl>
#include <parser/DynamicTilestore.hpp>
#include <parser/GameConfig.hpp>
#include <parser/LevelConfig.hpp>
#include <parser/TilesetConfig.hpp>

#include "editor/SpawnInfo.hpp"
#include "editor/model/MainWindowModel.hpp"
#include "editor/scene/GraphicsScenes.hpp"
#include "editor/ui_MainWindow.h"

class QCloseEvent;

namespace ctb {
namespace editor {

class MainWindow final : public QMainWindow {
    Q_OBJECT

   public:
    /// Init main window
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow() override = default;

   public slots:
    /// Opens a dialog to create a new game.
    void newGame();
    /// Opens a dialog to open an existing game.
    void openGame();
    /// Saves current game.
    void saveGame();
    /// Saves current game and starts CTB++ application
    void saveStartGame();

    /// Opens a dialog to add a new level.
    void addLevel();
    /// Removes the selected level.
    void removeLevel();
    /// Resizes the selected level.
    void resizeLevel();
    /// Opens a level background settings dialog
    void levelBackgroundSettings();
    /// Opens a level settings dialog.
    void levelSettings();

    /// Opens a dialog to add a new layer/tileset.
    void addLayer();
    /// Removes the sellected layer/tileset.
    void removeLayer();
    /// Opens a layer/tileset settings dialog.
    void layerSettings();
    /// Validates the game configuration.
    void validateGame();

    /// Shows the application's about dialog
    void about();

    /// Level combobox selected index changed event
    void cbLevelIdCurrentIndexChanged(int index);
    /// Layer/tileset combobox selected index changed event
    void cbLayerIdCurrentIndexChanged(int index);

   protected:
    /// Window show event
    void showEvent(QShowEvent* event) override;

    /// Qt mainwindow - UI elements
    std::unique_ptr<Ui::MainWindow> m_ui;

   private:
    /// Window close event
    void closeEvent(QCloseEvent* event) override;

    /// Init window menu and menu entries
    void createMenus();
    /// Init game ui and prepair backend
    void initGameUi();
    /// Resets the game UI and backend
    void resetGameUi();
    /// Updates all level and layer/tileset UI components
    void updateLevelUi();
    /// Updates all layer/tileset UI components
    void updateTilesetUi();

    /// Shows a quit message
    QMessageBox::StandardButton showQuitMessage();

    /// \brief Layer/Tileset callback
    ///
    /// \param action Action
    /// \param coordinate Selected tile coordinate
    void tilesetItemCallback(TilesetItemCallbackAction action,
                             TilesetScene::CoordinateT coordinate);

    // ------------------------------------------------------------------------

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

    // void saveAsGameConfig(const std::string& filename);

    /// \brief Adds the given level to the game.
    ///
    /// \param level The level to add
    /// \param setStatus Sets hasChanged status to true
    void addLevelConfig(LevelConfigT* level, bool setStatus = true);

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
    void addNewTilesetConfig(int layerPosition,
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

    /// Indicates whether there are changes
    bool hasChanges() { return m_model.hasChanges(); }
    /// Returns selected level index or -1.
    int getSelectedLevelIndex() { return m_model.getSelectedLevelIndex(); }
    /// Returns selected layer/tileset index or -1.
    int getSelectedTilesetIndex() { return m_model.getSelectedTilesetIndex(); }
    /// Returns selected tile id or -1.
    int getSelectedTileId() { return m_model.getSelectedTileId(); }
    /// Returns game config pointer
    GameConfigT* getGameConfig() { return m_model.getGameConfig(); }
    /// Return selected level config
    LevelConfigT* getCurrentLevel() { return m_model.getCurrentLevel(); }

    // ------------------------------------------------------------------------

    /// Indicates whether the editor is editable
    bool m_active{false};
    /// Temporary disable all events
    bool m_disableEvents{false};

    /// Backend
    MainWindowModel m_model;

    /// Path to the game application
    std::string m_GameAppPath{};

    // Qt menu and menu entries
    std::unique_ptr<QMenu> m_fileMenu{nullptr};
    std::unique_ptr<QMenu> m_editMenu{nullptr};
    std::unique_ptr<QMenu> m_helpMenu{nullptr};
    std::unique_ptr<QMenu> m_levelSubMenu{nullptr};
    std::unique_ptr<QMenu> m_layerSubMenu{nullptr};
    std::unique_ptr<QAction> m_newAct{nullptr};
    std::unique_ptr<QAction> m_openAct{nullptr};
    std::unique_ptr<QAction> m_saveAct{nullptr};
    std::unique_ptr<QAction> m_saveStartAct{nullptr};
    std::unique_ptr<QAction> m_exitAct{nullptr};
    std::unique_ptr<QAction> m_addLeveltAct{nullptr};
    std::unique_ptr<QAction> m_removeLevelAct{nullptr};
    std::unique_ptr<QAction> m_resizeLevelAct{nullptr};
    std::unique_ptr<QAction> m_levelBackgroundSettingsAct{nullptr};
    std::unique_ptr<QAction> m_levelSettingsAct{nullptr};
    std::unique_ptr<QAction> m_addLayerAct{nullptr};
    std::unique_ptr<QAction> m_removeLayerAct{nullptr};
    std::unique_ptr<QAction> m_layerSettingsAct{nullptr};
    std::unique_ptr<QAction> m_validateGameAct{nullptr};
    std::unique_ptr<QAction> m_aboutAct{nullptr};
    std::unique_ptr<QAction> m_aboutQtAct{nullptr};
};

}  // namespace editor
}  // namespace ctb

#endif  // MAINWINDOW_H
