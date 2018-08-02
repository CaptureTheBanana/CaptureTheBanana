// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <SDL.h>

#include "engine/Engine.hpp"
#include "engine/Window.hpp"
#include "engine/core/Game.hpp"
#include "engine/gui/Label.hpp"
#include "engine/gui/Statusbar.hpp"
#include "engine/misc/Highscores.hpp"
#include "engine/scene/Player.hpp"
#include "engine/util/Color.hpp"
#include "engine/util/Vector2d.hpp"

namespace ctb {
namespace engine {

int Statusbar::entryWidth = 170;

Statusbar::Statusbar() : m_currentLevelRectShow(true), m_lastRenderTicks(0) {
    setFPS(2);  // Slow blink
    setupUI();
}

void Statusbar::setFPS(int fps) {
    m_frameTimeout = static_cast<Uint32>(1000.0 / fps);
}

void Statusbar::updateCurrentLevelRect() {
    Uint32 ticks = SDL_GetTicks();
    float time = static_cast<float>(ticks - m_lastRenderTicks);

    // Check, if we have reached an update
    if (time > static_cast<float>(m_frameTimeout)) {
        m_currentLevelRectShow = !m_currentLevelRectShow;

        // Save current tick count
        m_lastRenderTicks = ticks;
    }

    // Update position deteminated from the current level
    int x = Window::getWindow().w() / 2 -
            ((Window::getEngine().getGame()->getLevelCount() / 2) * 50 + 15) +
            (Window::getEngine().getGame()->getCurrentLevelIndex()) * 50;
    m_currentLevelRect->setX(x);
}

void Statusbar::setupUI() {
    int w = Window::getWindow().w();
    int h = Window::getWindow().h();
    Game* game = Window::getEngine().getGame();

    int y_score = h - 54;
    int y_health = h - 29;

    int x = 60;
    Color red(255, 0, 0);

    // L2R players
    for (auto& player : game->getL2RPlayers()) {
        // Score label
        Label* l = new Label("std_18px", Highscores::formatScore(player->getScore()),
                             Vector2dT(x, y_score));
        m_scoreLabels.insert(std::make_pair(player, l));
        m_labels.push_back(l);

        // Health label
        l = new Label("std_18px", "100%", Vector2dT(x + 108, y_health));
        l->setAlignment(LabelAlignment::Right);
        m_healthLabels.insert(std::make_pair(player, l));
        m_labels.push_back(l);

        // red heart
        l = new Label("std_18px", static_cast<char>(3), Vector2dT(x, y_health));
        l->setColor(red);
        m_heartLabels.insert(std::make_pair(player, l));
        m_labels.push_back(l);

        x += entryWidth;
    }

    // r2l players
    x = w - 60;
    for (auto& player : game->getR2LPlayers()) {
        // Score label
        Label* l = new Label("std_18px", Highscores::formatScore(player->getScore()),
                             Vector2dT(x, y_score));
        l->setAlignment(LabelAlignment::Right);
        m_scoreLabels.insert(std::make_pair(player, l));
        m_labels.push_back(l);

        // Health label
        l = new Label("std_18px", "100%", Vector2dT(x, y_health));
        l->setAlignment(LabelAlignment::Right);
        m_healthLabels.insert(std::make_pair(player, l));
        m_labels.push_back(l);

        // red heart
        l = new Label("std_18px", static_cast<char>(3), Vector2dT(x - 108, y_health));
        l->setColor(red);
        m_heartLabels.insert(std::make_pair(player, l));
        m_labels.push_back(l);

        x -= entryWidth;
    }

    // the level indicator
    int levelCount = game->getLevelCount();
    SDL_Color color = {200, 200, 200, 255};
    int y = h - 52;  // (64-40) / 2 + 40
    x = w / 2 - (levelCount / 2) * 50 - 20;
    for (int level = 0; level < levelCount; level++) {
        SDL_Rect r = {x, y, 40, 40};
        Rect* rect = new Rect(r);
        rect->setColor(color);
        m_rects.push_back(rect);
        x += 50;
    }

    // The bars between the levels
    x = w / 2 - (levelCount / 2 - 1) * 50 - 30;
    y = h - 34;  // 32 + 2
    for (int level = 0; level < levelCount - 1; level++) {
        SDL_Rect r = {x, y, 10, 4};
        Rect* rect = new Rect(r);
        rect->setColor(color);
        m_rects.push_back(rect);
        x += 50;
    }

    // The blinky rect for the current level.
    x = w / 2 - ((levelCount / 2) * 50 + 15) + (game->getCurrentLevelIndex()) * 50;
    y = h - 47;  // (64-40) / 2 + 35
    SDL_Rect r = {x, y, 30, 30};
    m_currentLevelRect = new Rect(r);
    m_currentLevelRect->setColor({0, 0, 0, 255});
}

void Statusbar::render() {
    int w = Window::getWindow().w();
    int h = Window::getWindow().h();
    Game* game = Window::getEngine().getGame();
    auto* renderer = Window::getWindow().renderer();

    updateCurrentLevelRect();

    // Renders a transparent black rect over the bottom of the window
    SDL_Rect a = {0, h - 64, w, 64};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_RenderFillRect(renderer, &a);

    int y = h - 60;
    int x = 4;
    // Render the players and update their score and health labels
    for (auto& player : game->getL2RPlayers()) {
        Vector2dT p(x, y);
        player->renderStatic(p);
        m_scoreLabels[player]->setText(Highscores::formatScore(player->getScore()));
        m_healthLabels[player]->setText(std::to_string(player->getHealth()) + "%");
        x += entryWidth;

        // Update heart
        m_heartLabels[player]->setShow(!player->isHealing());
    }
    x = w - 60;
    for (auto& player : game->getR2LPlayers()) {
        Vector2dT p(x, y);
        player->renderStatic(p);
        m_scoreLabels[player]->setText(Highscores::formatScore(player->getScore()));
        m_healthLabels[player]->setText(std::to_string(player->getHealth()) + "%");
        x -= entryWidth;

        // Update heart
        m_heartLabels[player]->setShow(!player->isHealing());
    }

    // Render all labels
    for (auto& lbl : m_labels) {
        lbl->render();
    }

    // level indicator
    for (auto& rect : m_rects) {
        rect->render();
    }

    // If the black indicator should be shown, render it
    if (m_currentLevelRectShow) {
        m_currentLevelRect->render();
    }
}

Statusbar::~Statusbar() {
    for (auto& lbl : m_labels) {
        delete lbl;
    }
    for (auto& rect : m_rects) {
        delete rect;
    }
    delete m_currentLevelRect;
}

}  // namespace engine
}  // namespace ctb
