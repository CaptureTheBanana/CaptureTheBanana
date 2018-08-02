// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <algorithm>
#include <iostream>

#include "engine/input/Input.hpp"

#include <SDL.h>

namespace ctb {
namespace engine {

Input::Input() {
    // Set all states to false (released).
    m_lastStates.insert(std::make_pair(InputType::INPUT_LEFT, false));
    m_lastStates.insert(std::make_pair(InputType::INPUT_RIGHT, false));
    m_lastStates.insert(std::make_pair(InputType::INPUT_AIM_VERT, false));
    m_lastStates.insert(std::make_pair(InputType::INPUT_SHOOT, false));
    m_lastStates.insert(std::make_pair(InputType::INPUT_JUMP, false));
    m_lastStates.insert(std::make_pair(InputType::INPUT_PAUSE, false));
    m_lastStates.insert(std::make_pair(InputType::INPUT_SELECT, false));
    m_lastStates.insert(std::make_pair(InputType::INPUT_RETURN, false));
    m_lastStates.insert(std::make_pair(InputType::INPUT_UP, false));
    m_lastStates.insert(std::make_pair(InputType::INPUT_DOWN, false));
    m_lastStates.insert(std::make_pair(InputType::INPUT_TEAM_1, false));
    m_lastStates.insert(std::make_pair(InputType::INPUT_TEAM_2, false));
    m_lastStates.insert(std::make_pair(InputType::INPUT_HEAL, false));
    m_lastStates.insert(std::make_pair(InputType::INPUT_TAUNT, false));
    m_lastStates.insert(std::make_pair(InputType::INPUT_DROP_WEAPON, false));
}

uint64_t Input::idCounter = 1;

uint64_t Input::register_handler(InputHandler& handler) {
    m_handlers[idCounter] = handler;
    return idCounter++;
}

void Input::deregister_handler(uint64_t id) {
    auto it = m_handlers.find(id);
    if (it != m_handlers.end()) {
        m_handlers.erase(it);
    }
}

void Input::call_handlers(InputType type, bool state, float angle) {
    // Just call handlers, if the state has changed, or at aiming events
    if (m_lastStates[type] != state || type == InputType::INPUT_AIM_VERT) {
        m_lastStates[type] = state;
        for (std::pair<uint64_t, InputHandler> item : m_handlers) {
            item.second(type, this, state, angle);
        }
    }
}

}  // namespace engine
}  // namespace ctb
