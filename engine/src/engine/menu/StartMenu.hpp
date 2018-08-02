// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_MENU_STARTMENU_HPP
#define ENGINE_MENU_STARTMENU_HPP

#include <vector>

#include <parser/GameParser.hpp>

#include "engine/core/Game.hpp"
#include "engine/graphics/Rect.hpp"
#include "engine/menu/Menu.hpp"

namespace ctb {
namespace engine {

class Label;

/// Enumerates all actions possible in this menu.
enum class StartMenuAction { Start, Highscores, Keyboard, EditGame, Credits, Quit };

/// \brief The Start menu to launch the game, see credits and highscores, team assignments
///        and launch of the game editor
class StartMenu : public Menu {
   public:
    explicit StartMenu(parser::GameConfig* config);

    void render() override;

    /// Overrides the Menu::registerInputs. Adds functionality to add team assignments
    /// to each input.
    void registerInputs(const std::vector<Input*> inputs) override;

    /// Input handler. Specialiced to map all inputs to players.
    void handleInput(InputType type, const Input* input, bool /*state*/, float /*angle*/) override;

    /// Set the frame rate for the blinking
    void setFPS(int fps);

    ~StartMenu() override;

   private:
    /// Sets up all possible players. Put them into the m_playersUnused.
    void setupPlayers();

    /// Setup all needed ui elements.
    void setupUI();

    /// Update the blink from the selected entry.
    void updateBlink();

    /// If a player presses select, this function is called to perform the right action.
    void select();

    /// Enables or disables the keyboard as an input device.
    void toggleKeyboard();

    /// Method to render the controller team assignment in the table
    void renderPlayers();

    /// Set the height of the divider right for the amount of inputs
    void updateDividerHeight();

    /// Updates the start enty if it is allowed to select it.
    void updateStartEntry();

    /// The gameconfig is needed to create all players from the level config.
    parser::GameConfig* m_gameconfig;

    /// List of all players, that are not mapped to inputs, so not used
    std::vector<Player*> m_playersUnused;

    /// List of all players, that are mapped to inputs.
    std::vector<Player*> m_playersUsed;

    /// Ticks count when the last frame was rendered
    Uint32 m_lastRenderTicks;

    /// Timeout between frames
    Uint32 m_frameTimeout;

    /// The header label witht the ctb++ title
    Label* m_lblHeader;

    /// The start game slabel
    Label* m_lblStart;

    /// The toggle keyboard label
    Label* m_lblKeyboard;

    /// All labels that have an select action mapped to them
    std::vector<std::pair<Label*, StartMenuAction>> m_menuLabels;

    /// Label for the R2L team
    Label* m_lblTeamR2L;

    /// Label for the L2R team
    Label* m_lblTeamL2R;

    /// The vertical divider between the teams
    Rect* m_divider;

    /// The selected action index
    unsigned int m_selected;

    /// Save, if the start label is enabled
    bool m_startEnabled;

    /// Save if this menu should free all used players or give the
    /// ownership to the game
    bool m_freePlayersUsed;
};

}  // namespace engine
}  // namespace ctb
#endif
