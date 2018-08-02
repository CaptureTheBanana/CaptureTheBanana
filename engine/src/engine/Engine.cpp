// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <parser/GameParser.hpp>

#include "common/Exceptions.hpp"
#include "engine/Engine.hpp"
#include "engine/Window.hpp"
#include "engine/core/Game.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/menu/Menu.hpp"
#include "engine/menu/StartMenu.hpp"

namespace ctb {
namespace engine {

Engine::Engine(const std::string& gamefile)
    : m_gamefile(gamefile), m_game(nullptr), m_config(nullptr) {
    // Load gameconfig from game
    m_config = ctb::parser::parseGame(gamefile);

    // Validate game configuration
    auto info = ctb::parser::validateGameConfig(m_config);
    if (info.status != ctb::parser::GameValidatorStatus::kOk) {
        throw GameValidatorExceptionT(ctb::parser::to_string(info));
    }
}

void Engine::restart() {
    delete m_game;
    m_game = new Game(m_config);

    // Init startmenu
    Menu* menu = new StartMenu(m_config);
    menu->registerInputs(Window::getInputManager().getInputs());
    Window::getWindow().addMenu(menu);
}

void Engine::update() {
    // update game
    m_game->update();

    // If a menu has to be rendered, make sure the game is nur running.
    Menu* currentMenu = Window::getWindow().getCurrentMenu();
    if (currentMenu != nullptr && m_game->getGameState() == GameState::Running) {
        m_game->pause();
    }
    m_game->render();
}

parser::GameConfig* Engine::getGameConfig() {
    return m_config;
}

void Engine::pauseGame(Player* player) {
    auto* pauseMenu = new PauseMenu(player);
    Window::getWindow().addMenu(pauseMenu);
}

Engine::~Engine() {
    // Delete all resources
    delete m_game;
    delete m_config;
}

}  // namespace engine
}  // namespace ctb
