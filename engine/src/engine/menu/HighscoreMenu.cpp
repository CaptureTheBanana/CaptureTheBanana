// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <string>

#include "engine/Engine.hpp"
#include "engine/Window.hpp"
#include "engine/gui/Label.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/menu/HighscoreMenu.hpp"
#include "engine/misc/Highscores.hpp"
#include "engine/util/Vector2d.hpp"

namespace ctb {
namespace engine {

HighscoreMenu::HighscoreMenu() {
    registerInputs(Window::getInputManager().getInputs());
    setupUI();
}

void HighscoreMenu::setupUI() {
    int w = Window::getWindow().w();
    int x = w / 2;
    int y = Window::getWindow().h() / 10;

    // header label in the center.
    Label* header = new Label("std_12px", "Highscores", Vector2dT(x, y), 3);
    header->setAlignment(LabelAlignment::Center);
    m_labels.push_back(header);

    // Loop over all highscores. Add labels for each one:
    // X. 003392 super user
    int count = 1;
    x = w / 3;
    y += 50;
    for (auto& entry : Highscores::getScores()) {
        std::string text = std::to_string(count);
        // Align all scores right
        if (count < 10) {
            text += ".  ";
        } else {
            text += ". ";
        }
        text += Highscores::formatScore(entry.second) + " " + entry.first;
        m_labels.push_back(new Label("std_12px", text, Vector2dT(x, y), 2));
        y += 35;
        count++;
    }

    // Maybe there are no scores. Put a nice message then..
    if (Highscores::getScores().empty()) {
        y += 20;
        x = w / 2;
        Label* l = new Label("std_12px", "Oh, no scores here... You can be the first!",
                             Vector2dT(x, y), 2);
        l->setAlignment(LabelAlignment::Center);
        m_labels.push_back(l);
    }
}

void HighscoreMenu::render() {
    // Just render thebackground and all labels
    renderBackground();
    for (auto& lbl : m_labels) {
        lbl->render();
    }
}

void HighscoreMenu::handleInput(InputType type,
                                const Input* /*input*/,
                                bool state,
                                float /*angle*/) {
    if (state && type == InputType::INPUT_RETURN) {
        Window::getWindow().closeCurrentMenu();
    }
}

HighscoreMenu::~HighscoreMenu() {
    for (auto& lbl : m_labels) {
        delete lbl;
    }
}

}  // namespace engine
}  // namespace ctb
