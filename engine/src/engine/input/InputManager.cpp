// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <SDL.h>
#include <parser/GameConfig.hpp>

#include "engine/Engine.hpp"
#include "engine/Window.hpp"
#include "engine/core/GC.hpp"
#include "engine/core/Game.hpp"
#include "engine/input/Controller.hpp"
#include "engine/input/Input.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/input/Keyboard.hpp"

namespace ctb {
namespace engine {

InputManager::InputManager(parser::GameConfig* config) : m_addKeyboard(false), m_keyboard(nullptr) {
    // Get all available inputs
    setupDefaultInputs(config->getInputsFile());
}

bool InputManager::update() {
    bool quit = false;
    SDL_Event e;

    // Process events
    while (SDL_PollEvent(&e) == 1) {
        switch (e.type) {
            case SDL_MOUSEBUTTONDOWN: {
                if (!Window::isDebug() && e.button.button == 1) {
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                }
                break;
            }
            case SDL_QUIT: {
                quit = true;
                break;
            }
            case SDL_KEYDOWN: {
                // TODO(felix): Move this block into 'Engine'
                if (Window::isDebug()) {
                    if (e.key.keysym.sym == SDLK_F12) {
                        Window::getWindow().getEngine().getGame()->endGame(Team::R2L);
                    } else if (e.key.keysym.sym == SDLK_F11) {
                        Window::getWindow().getEngine().getGame()->endGame(Team::L2R);
                    }
                }
                break;
            }
        }

        for (auto& i : m_inputs) {
            i->handleSdlEvent(e);
        }
    }

    // Give this event the current menu
    Menu* currentMenu = Window::getWindow().getCurrentMenu();
    if (currentMenu != nullptr) {
        currentMenu->handleSdlEvent(e);
    }

    // Propagate the keys to all inputs
    const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
    for (auto& i : m_inputs) {
        i->pollInput(currentKeyStates);
    }

    // add keyboard outside of the iterations over m_inputs to this vector.
    if (m_addKeyboard) {
        m_inputs.push_back(m_keyboard);
        m_addKeyboard = false;
    }

    // window realeases the mouse when ESC is pressed
    if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }
    return quit;
}

void InputManager::setupDefaultInputs(const std::string& controllerMapFile) {
    // load input mappings
    if (SDL_GameControllerAddMappingsFromFile(controllerMapFile.c_str()) == -1) {
        std::cout << "Cannot parse " << controllerMapFile << ": " << SDL_GetError() << std::endl;
        throw std::runtime_error("SDL error");
    }
    int joysticks = SDL_NumJoysticks();

    if (joysticks <= 1) {
        m_keyboard = new Keyboard();
        m_inputs.push_back(m_keyboard);
    }
    for (int id = 0; id < joysticks; id++) {
        if (SDL_IsGameController(id)) {
            SDL_GameController* controller = SDL_GameControllerOpen(id);
            if (nullptr != controller) {
                m_inputs.push_back(new Controller(controller));
            }
        }
    }
}

void InputManager::addKeyboard() {
    if (!hasKeyboard()) {
        m_keyboard = new Keyboard();
        m_addKeyboard = true;
    }
}

void InputManager::removeKeyboard() {
    if (hasKeyboard()) {
        auto it = std::find(m_inputs.begin(), m_inputs.end(), m_keyboard);
        m_inputs.erase(it);
        GC::add(m_keyboard);
        m_keyboard = nullptr;
    }
}

InputManager::~InputManager() {
    // delete inputs
    for (auto& i : m_inputs) {
        delete i;
        i = nullptr;
    }
}

}  // namespace engine
}  // namespace ctb
