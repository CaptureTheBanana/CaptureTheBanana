// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <algorithm>
#include <string>

#include "engine/Engine.hpp"
#include "engine/Window.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/menu/EndMenu.hpp"
#include "engine/menu/NewHighscoreMenu.hpp"
#include "engine/misc/Highscores.hpp"
#include "engine/scene/Player.hpp"
#include "engine/util/Vector2d.hpp"

namespace ctb {
namespace engine {

EndMenu::EndMenu(Team teamWon)
    : m_team(teamWon), m_divider(nullptr), m_checkedForHighscores(false) {
    setupUI();
    registerInputs(Window::getInputManager().getInputs());
}

void EndMenu::setupUI() {
    // Header label with the winning team
    std::string s;
    if (m_team == Team::R2L) {
        s = "Team R2L has won!";
    } else {
        s = "Team L2R has won!";
    }
    int w = Window::getWindow().w();
    int h = Window::getWindow().h();
    Label* header = new Label("std_12px", s, Vector2dT(w / 2, h / 10), 3);
    header->setAlignment(LabelAlignment::Center);
    m_labels.push_back(header);

    int y = h / 10 + 100;
    // R2L vector and label
    m_r2l = Window::getEngine().getGame()->getR2LPlayers();
    std::sort(m_r2l.begin(), m_r2l.end(), PlayerScoreSorter());
    m_labels.push_back(new Label("std_12px", "Team R2L", Vector2dT(w / 2 - 250, y), 2));

    // L2R
    m_l2r = Window::getEngine().getGame()->getL2RPlayers();
    std::sort(m_l2r.begin(), m_l2r.end(), PlayerScoreSorter());
    m_labels.push_back(new Label("std_12px", "Team L2R", Vector2dT(w / 2 + 50, y), 2));

    // The divider
    int tableLength = static_cast<int>(std::max(m_r2l.size(), m_l2r.size()));
    SDL_Rect r = {w / 2 - 3, y - 25, 6, tableLength * 60 + 100};
    m_divider = new Rect(r);

    // 1. 2. 3. for r2l
    y += 28;
    int count = 1;
    std::string text;
    for (auto& player : m_r2l) {
        text = std::to_string(count) + ".";
        m_labels.push_back(new Label("std_18px", text, Vector2dT(w / 2 - 250, y + count * 60)));
        m_labels.push_back(new Label("std_18px", Highscores::formatScore(player->getScore()),
                                     Vector2dT(w / 2 - 150, y + count * 60)));
        count++;
    }

    // Same for l2r
    count = 1;
    for (auto& player : m_l2r) {
        text = std::to_string(count) + ".";
        m_labels.push_back(new Label("std_18px", text, Vector2dT(w / 2 + 50, y + count * 60)));
        m_labels.push_back(new Label("std_18px", Highscores::formatScore(player->getScore()),
                                     Vector2dT(w / 2 + 150, y + count * 60)));
        count++;
    }
}

void EndMenu::render() {
    // Check, if one new highscore was reached.
    if (!m_checkedForHighscores) {
        m_checkedForHighscores = true;
        checkForHighscores();
        return;
    }

    // render background and labels
    renderBackground();
    for (auto& lbl : m_labels) {
        lbl->render();
    }

    if (m_divider != nullptr) {
        m_divider->render();
    }

    // Render players
    int y = Window::getWindow().h() / 10 + 90;
    int count = 1;
    int w = Window::getWindow().w();
    for (auto& player : m_r2l) {
        Vector2dT pos = {w / 2 - 210, y + count * 60};
        player->renderStatic(pos);
        count++;
    }
    count = 1;
    for (auto& player : m_l2r) {
        Vector2dT pos = {w / 2 + 85, y + count * 60};
        player->renderStatic(pos);
        count++;
    }
}

void EndMenu::checkForHighscores() {
    // Because the players are sorted, look for the both top scores
    uint64_t r2l_score = 0;
    uint64_t l2r_score = 0;

    if (!m_r2l.empty()) {
        r2l_score = m_r2l.front()->getScore();
    }
    if (!m_l2r.empty()) {
        l2r_score = m_l2r.front()->getScore();
    }

    // If both teams have a top score, check for a new highscore
    if (r2l_score != 0 || l2r_score != 0) {
        uint64_t score = 0;
        Player* player = nullptr;
        // Which tea have the higher score?
        if (r2l_score >= l2r_score) {
            score = r2l_score;
            player = m_r2l.front();
        } else {
            score = l2r_score;
            player = m_l2r.front();
        }

        // Check if the score *is* a highscore
        if (Highscores::isHighscore(score)) {
            // Open Highscore menu for the player
            NewHighscoreMenu* menu = new NewHighscoreMenu(player);
            Window::getWindow().addMenu(menu);
        }
    }
}

void EndMenu::handleInput(InputType type, const Input* /*input*/, bool state, float /*angle*/) {
    if (type == InputType::INPUT_SELECT && state) {
        Window::getWindow().closeCurrentMenu();
        Window::getEngine().restart();
    }
}

EndMenu::~EndMenu() {
    for (auto& lbl : m_labels) {
        delete lbl;
    }
    delete m_divider;
}

}  // namespace engine
}  // end namespace ctb
