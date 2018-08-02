// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <iostream>
#include <utility>

#include "engine/Window.hpp"
#include "engine/input/Keyboard.hpp"

namespace ctb {
namespace engine {

Keyboard::Keyboard() {
    // Initialize all assignments
    m_keyMap.insert(std::make_pair(InputType::INPUT_LEFT,
                                   std::vector<unsigned int>{SDL_SCANCODE_LEFT, SDL_SCANCODE_A}));
    m_keyMap.insert(std::make_pair(InputType::INPUT_RIGHT,
                                   std::vector<unsigned int>{SDL_SCANCODE_RIGHT, SDL_SCANCODE_D}));
    m_keyMap.insert(std::make_pair(InputType::INPUT_UP,
                                   std::vector<unsigned int>{SDL_SCANCODE_UP, SDL_SCANCODE_W}));
    m_keyMap.insert(std::make_pair(InputType::INPUT_DOWN,
                                   std::vector<unsigned int>{SDL_SCANCODE_DOWN, SDL_SCANCODE_S}));
    m_keyMap.insert(
        std::make_pair(InputType::INPUT_JUMP, std::vector<unsigned int>{SDL_SCANCODE_SPACE}));
    m_keyMap.insert(
        std::make_pair(InputType::INPUT_PAUSE, std::vector<unsigned int>{SDL_SCANCODE_ESCAPE}));
    m_keyMap.insert(
        std::make_pair(InputType::INPUT_SELECT, std::vector<unsigned int>{SDL_SCANCODE_RETURN}));
    m_keyMap.insert(
        std::make_pair(InputType::INPUT_RETURN, std::vector<unsigned int>{SDL_SCANCODE_BACKSPACE}));
    m_keyMap.insert(std::make_pair(InputType::INPUT_TEAM_1,
                                   std::vector<unsigned int>{SDL_SCANCODE_1, SDL_SCANCODE_LEFT}));
    m_keyMap.insert(std::make_pair(InputType::INPUT_TEAM_2,
                                   std::vector<unsigned int>{SDL_SCANCODE_2, SDL_SCANCODE_RIGHT}));
    m_keyMap.insert(
        std::make_pair(InputType::INPUT_TAUNT, std::vector<unsigned int>{SDL_SCANCODE_LALT}));
    m_keyMap.insert(std::make_pair(
        InputType::INPUT_HEAL, std::vector<unsigned int>{SDL_SCANCODE_RCTRL, SDL_SCANCODE_LCTRL}));
    m_keyMap.insert(
        std::make_pair(InputType::INPUT_DROP_WEAPON, std::vector<unsigned int>{SDL_SCANCODE_F}));
}

void Keyboard::handleSdlEvent(const SDL_Event& event) {
    // Get the mouse's vertical offset
    if (event.type == SDL_MOUSEMOTION) {
        int dy = event.motion.y;
        int h = Window::getWindow().h();
        int y = h - dy;

        // compute the aiming angle
        float angle = static_cast<float>(y) / static_cast<float>(h) * 180.0f - 90.0f;

        call_handlers(InputType::INPUT_AIM_VERT, true, angle);

        // sensibility for triggers (shooting) is either on or off:
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            call_handlers(InputType::INPUT_SHOOT, true);
        }
    } else if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            call_handlers(InputType::INPUT_SHOOT, false);
        }
    }
}

void Keyboard::pollInput(const Uint8* keyStates) {
    for (auto pair : m_keyMap) {
        bool state = false;
        for (auto key : pair.second) {
            state = state || static_cast<bool>(keyStates[key]);
        }
        // in call_handlers is checked, if the state has changes. If not, no handler gets called.
        call_handlers(pair.first, state);
    }
}

}  // namespace engine
}  // namespace ctb
