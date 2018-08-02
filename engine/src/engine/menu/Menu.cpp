// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <iostream>
#include <utility>

#include <SDL.h>

#include "engine/Engine.hpp"
#include "engine/Window.hpp"
#include "engine/menu/Menu.hpp"

namespace ctb {
namespace engine {

void Menu::renderBackground() {
    // Renders a full black rect over the complete window.
    SDL_Rect a = {0, 0, Window::getWindow().w(), Window::getWindow().h()};
    auto* renderer = Window::getWindow().renderer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 210);
    SDL_RenderFillRect(renderer, &a);
}

void Menu::registerInput(Input* input) {
    // Check, if the input is already registered.
    if (m_inputHandlerIds.count(input) == 0) {
        // Create the handler
        InputHandler handler =
            std::bind(&Menu::_handleInput, this, std::placeholders::_1, std::placeholders::_2,
                      std::placeholders::_3, std::placeholders::_4);
        // Save the handler id mapped to the input pointer
        m_inputHandlerIds[input] = input->register_handler(handler);
    }
}

void Menu::registerInputs(std::vector<Input*> inputs) {
    // class registerInput for every input in the vector.
    for (Input*& input : inputs) {
        registerInput(input);
    }
}

void Menu::_handleInput(InputType type, const Input* input, bool state, float angle) {
    // Just execute the handleInput, if this menu is the current one.
    if (this == Window::getWindow().getCurrentMenu()) {
        handleInput(type, input, state, angle);
    }
}

void Menu::deregisterAllHandlers() {
    for (std::pair<Input*, uint64_t> item : m_inputHandlerIds) {
        item.first->deregister_handler(item.second);
    }
}

Menu::~Menu() {
    deregisterAllHandlers();
}

}  // namespace engine
}  // namespace ctb
