// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/menu/NewHighscoreMenu.hpp"
#include "engine/Engine.hpp"
#include "engine/Window.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/input/Keyboard.hpp"
#include "engine/misc/Highscores.hpp"
#include "engine/util/Vector2d.hpp"

namespace ctb {
namespace engine {

NewHighscoreMenu::NewHighscoreMenu(Player* player) : m_name(""), m_player(player) {
    setupUI();
    // Just the input from the player
    registerInput(player->getInput());

    // Special case: If a controller player can type the name, enable enter in addition
    // to the controller INPUT_SELECT aquivialent, because when entering text, enter is
    // more natural to use then having to press a button on the controller
    if (Window::getInputManager().hasKeyboard() &&
        player->getInput()->getType() != InputDeviceType::Keyboard) {
        registerInput(Window::getInputManager().getKeyboard());
    }

    // Enable text input for the SDL event SDL_TEXTINPUT
    SDL_StartTextInput();
}

void NewHighscoreMenu::setupUI() {
    // Header text. Special one, if this score is now nr. 1 in the highscore list
    int place = static_cast<int>(Highscores::isHighscore(m_player->getScore()));
    std::string headerText;
    if (place == 1) {
        headerText = "New Highscore!";
    } else {
        headerText = std::to_string(place) + ". Place in Highscores!";
    }

    int x = Window::getWindow().w() / 2;
    int y = Window::getWindow().h() / 10;

    Label* header = new Label("std_12px", headerText, Vector2dT(x, y), 3);
    header->setAlignment(LabelAlignment::Center);
    m_labels.push_back(header);

    // The score
    y += 50;
    Label* score =
        new Label("std_12px", Highscores::formatScore(m_player->getScore()), Vector2dT(x, y), 2);
    score->setAlignment(LabelAlignment::Center);
    m_labels.push_back(score);

    // The prompt
    x = 100;
    y = Window::getWindow().h() / 2;
    m_labels.push_back(new Label("std_12px", "Enter name:", Vector2dT(x, y), 2));

    // The name label and cursor
    x = 400;
    m_lblName = new Label("std_12px", m_name, Vector2dT(x, y), 2);
    m_labels.push_back(m_lblName);
    m_lblCursor = new Label("std_12px", "_", Vector2dT(x, y), 2);
    m_labels.push_back(m_lblCursor);
}

void NewHighscoreMenu::render() {
    // render background and labels
    renderBackground();
    for (auto& lbl : m_labels) {
        lbl->render();
    }
    // Render the player who reached the score
    Vector2dT p(Window::getWindow().w() / 2 - 27, Window::getWindow().h() / 10 + 150);
    m_player->renderStatic(p);
}

void NewHighscoreMenu::setCursorPosition() {
    int x = 400 + static_cast<int>(m_name.length()) * 24;
    int y = Window::getWindow().h() / 2;
    m_lblCursor->setPosition(Vector2dT(x, y));
}

void NewHighscoreMenu::handleSdlEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        // Handle backspace
        if (event.key.keysym.sym == SDLK_BACKSPACE && m_name.length() > 0) {
            m_name.pop_back();
            m_lblName->setText(m_name);
            setCursorPosition();
        }
    } else if (event.type == SDL_TEXTINPUT) {
        if (m_name.length() < 16) {
            m_name += event.text.text;
            m_lblName->setText(m_name);
            setCursorPosition();
        }
    }
}

void NewHighscoreMenu::handleInput(InputType type,
                                   const Input* /*input*/,
                                   bool state,
                                   float /*angle*/) {
    if (m_name.length() > 0 && state && type == InputType::INPUT_SELECT) {
        Highscores::insert(m_name, m_player->getScore());
        Window::getWindow().closeCurrentMenu();
    }
}

NewHighscoreMenu::~NewHighscoreMenu() {
    // Disable text input
    SDL_StopTextInput();
    for (auto& lbl : m_labels) {
        delete lbl;
    }
}

}  // namespace engine
}  // namespace ctb
