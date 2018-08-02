// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_MENU_NEWHIGHSCOREMENU_HPP
#define ENGINE_MENU_NEWHIGHSCOREMENU_HPP

#include <string>
#include <vector>

#include <SDL.h>

#include "engine/gui/Label.hpp"
#include "engine/menu/Menu.hpp"
#include "engine/scene/Player.hpp"

namespace ctb {
namespace engine {

/// Menu to enter the name for the player, who has a new highscore
class NewHighscoreMenu : public Menu {
   public:
    explicit NewHighscoreMenu(Player* player);

    /// Render the menu
    void render() override;

    /// Handles SDL events. Used for entering text.
    void handleSdlEvent(const SDL_Event& event) override;

    ~NewHighscoreMenu() override;

   private:
    /// Input handler
    void handleInput(InputType type, const Input* input, bool state, float angle) override;

    /// Create all labels needed
    void setupUI();

    /// Sets the cursor position after the typed name.
    void setCursorPosition();

    /// The entered name
    std::string m_name;

    /// The player with the new highscore
    Player* m_player;

    /// Contains all labels for the menu.
    std::vector<Label*> m_labels;

    /// Label for the entered name
    Label* m_lblName;

    /// Label for the cursor
    Label* m_lblCursor;
};

}  // namespace engine
}  // namespace ctb

#endif
