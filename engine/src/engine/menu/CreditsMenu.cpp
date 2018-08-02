// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/menu/CreditsMenu.hpp"
#include "engine/Engine.hpp"
#include "engine/gui/Label.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/util/Vector2d.hpp"

namespace ctb {
namespace engine {

CreditsMenu::CreditsMenu() {
    registerInputs(Window::getInputManager().getInputs());
    setupUI();
}

void CreditsMenu::render() {
    // render background and labels
    renderBackground();
    for (auto& lbl : m_labels) {
        lbl->render();
    }
}

void CreditsMenu::setupUI() {
    // Create header label
    int x = Window::getWindow().w() / 2;
    int y = Window::getWindow().h() / 10;
    m_labels.push_back(new Label("std_12px", "Credits", Vector2dT(x, y), 3));

    // Create labels for each contributor
    std::vector<std::string> names{"A. Domme",        "M. Eisoldt",     "F. Haeusler",
                                   "M. Huendorf",     "J. Juelke",      "N. Kirchner",
                                   "M. Kl. Piening",  "J. Sangmeister", "V. Steinbruegge",
                                   "F. Stutzenstein", "S. Schupp"};
    y += 70;
    for (auto& name : names) {
        m_labels.push_back(new Label("std_12px", name, Vector2dT(x, y), 2));
        y += 45;
    }

    // Set alignments to center for every label.
    for (auto& lbl : m_labels) {
        lbl->setAlignment(LabelAlignment::Center);
    }
}

void CreditsMenu::handleInput(InputType type, const Input* /*input*/, bool state, float /*angle*/) {
    // Just listen to key presses for return
    if (type == InputType::INPUT_RETURN && state) {
        Window::getWindow().closeCurrentMenu();
    }
}

CreditsMenu::~CreditsMenu() {
    // delete all labels.
    for (auto* lbl : m_labels) {
        delete lbl;
    }
}

}  // namespace engine
}  // namespace ctb
