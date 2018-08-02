// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_MENU_CREDITSMENU_HPP
#define ENGINE_MENU_CREDITSMENU_HPP

#include <vector>

#include "engine/input/Input.hpp"
#include "engine/menu/Menu.hpp"

namespace ctb {
namespace engine {

class Label;

/// Displays all contributors to this awesome game!
class CreditsMenu : public Menu {
   public:
    CreditsMenu();

    /// Renders the menu
    void render() override;

    ~CreditsMenu() override;

   private:
    /// Sets up the ui: Create labels for the menu.
    void setupUI();

    /// Input handler.
    void handleInput(InputType type, const Input* input, bool state, float angle) override;

    /// Contains all labels for the menu.
    std::vector<Label*> m_labels;
};

}  // namespace engine
}  // namespace ctb
#endif
