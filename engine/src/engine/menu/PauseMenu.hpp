// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_MENU_PAUSEMENU_HPP
#define ENGINE_MENU_PAUSEMENU_HPP

#include <vector>

#include "engine/input/Input.hpp"
#include "engine/menu/Menu.hpp"

namespace ctb {
namespace engine {

class Label;
class Player;

/// Menu for pausing the game
class PauseMenu : public Menu {
   public:
    /// The pause menu. Pauses the game on creation and resumes it at close.
    ///
    /// \param Player* player The player that caused the menu.
    explicit PauseMenu(Player* player);

    /// Renders the menu
    void render() override;

    ~PauseMenu() override;

   private:
    /// Input handler
    void handleInput(InputType type, const Input* input, bool state, float angle) override;

    /// Create all labels needed
    void setupUI();

    /// All labels used in this menu
    std::vector<Label*> m_labels;

    /// The player that caused this menu.
    Player* m_player;
};

}  // namespace engine
}  // namespace ctb
#endif
