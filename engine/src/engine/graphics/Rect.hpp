// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_GRAPHIC_RECT_HPP
#define ENGINE_GRAPHIC_RECT_HPP

#include <SDL.h>

#include "engine/graphics/SDLRenderable.hpp"

namespace ctb {
namespace engine {

/// Represents a filled rectangle that can be rendered
class Rect : public SDLRenderable {
   public:
    explicit Rect(SDL_Rect r) : SDLRenderable(), m_rect(r), m_color({255, 255, 255, 255}) {}

    /// Renders the rectangle
    void render() override;

    /// Setter for the rect
    void setRect(const SDL_Rect r) { m_rect = r; }

    /// Setter for the color
    void setColor(const SDL_Color c) { m_color = c; }

    /// Setter for the width
    void setW(const int w) { m_rect.w = w; }

    /// Setter for the height
    void setH(const int h) { m_rect.h = h; }

    /// Setter for the x position
    void setX(const int x) { m_rect.x = x; }

    /// Setter for the y position
    void setY(const int y) { m_rect.y = y; }

    ~Rect() override = default;

   private:
    /// The SDL rect that class is wrapped around
    SDL_Rect m_rect;

    /// The color to render m_rect
    SDL_Color m_color;
};

}  // namespace engine
}  // namespace ctb
#endif
