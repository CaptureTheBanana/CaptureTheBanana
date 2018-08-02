// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/menu/PauseMenu.hpp"
#include "engine/Engine.hpp"
#include "engine/Window.hpp"
#include "engine/core/Game.hpp"
#include "engine/gui/Label.hpp"
#include "engine/scene/Player.hpp"
#include "engine/util/Vector2d.hpp"

namespace ctb {
namespace engine {

PauseMenu::PauseMenu(Player* player) : m_player(player) {
    Window::getEngine().getGame()->pause();
    setupUI();
    registerInput(player->getInput());
}

void PauseMenu::setupUI() {
    int w = Window::getWindow().w();
    int h = Window::getWindow().h();
    // header label
    m_labels.push_back(new Label("std_12px", "Game paused by", Vector2dT(w / 2, h / 7), 3));

    // Little info how to continue based on the input device type
    std::string text;
    if (m_player->getInput()->getType() == InputDeviceType::Controller) {
        text = "Press B to continue";
    } else {
        text = "Press BACKSPACE to continue";
    }
    m_labels.push_back(new Label("std_12px", text, Vector2dT(w / 2, (h * 9) / 10), 2));

    // Everything centered
    for (auto& lbl : m_labels) {
        lbl->setAlignment(LabelAlignment::Center);
    }
}

void PauseMenu::render() {
    // Render background, labels and the player
    renderBackground();
    for (auto& lbl : m_labels) {
        lbl->render();
    }
    Vector2dT p(Window::getWindow().w() / 2 - 24, Window::getWindow().h() / 7 + 100);
    m_player->renderStatic(p);
}

void PauseMenu::handleInput(InputType type, const Input* input, bool state, float /*angle*/) {
    // Check, if the return button was pressed from the input that opened this menu.
    if (type == InputType::INPUT_RETURN && state && input == m_player->getInput()) {
        Window::getEngine().getGame()->resume();
        Window::getWindow().closeCurrentMenu();
    }
}

PauseMenu::~PauseMenu() {
    // Delete all created labels.
    for (auto& lbl : m_labels) {
        delete lbl;
    }
}

}  // namespace engine
}  // namespace ctb
