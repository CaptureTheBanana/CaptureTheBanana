// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/graphics/Rect.hpp"
#include "engine/Window.hpp"

namespace ctb {
namespace engine {

void Rect::render() {
    SDL_SetRenderDrawColor(Window::getWindow().renderer(), m_color.r, m_color.g, m_color.b,
                           m_color.a);
    SDL_RenderFillRect(Window::getWindow().renderer(), &m_rect);
}

}  // namespace engine
}  // namespace ctb
