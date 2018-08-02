// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_GRAPHIC_TEXTUREBASEDRENDERABLE_HPP
#define ENGINE_GRAPHIC_TEXTUREBASEDRENDERABLE_HPP

#include <SDL.h>

#include "engine/graphics/SDLRenderable.hpp"
#include "engine/util/Vector2d.hpp"

namespace ctb {
namespace engine {

/**
 * @brief Implementation of basic rendering functionality
 *        using SDL functionalities.
 */
class TextureBasedRenderable : public SDLRenderable {
   public:
    /**
     * @brief TextureBasedRenderable renders a SDL-Texture in a Window
     *
     * @param texture       Texture to render
     */
    explicit TextureBasedRenderable(SDL_Texture* texture);

    /// Render method
    void render() override;

    /// Width
    virtual int width() const { return m_targetRect.w; }

    /// Height
    virtual int height() const { return m_targetRect.h; }

    /// X position in screen coordinates
    virtual int x() const { return m_position.x; }

    /// Y position in screen coordintes
    virtual int y() const { return m_position.y; }

    /// Returns the current position
    Vector2dT position() const { return m_position; }

    /// Sets the position of the renderable in world coordinates
    virtual void setPosition(const Vector2dT& v);

    /// Renders the tile array according to the given offset
    void setOffset(const Vector2dT& offset) { m_offset = offset; }

    /// Destructor
    ~TextureBasedRenderable() override;

   protected:
    /// Computes the upper left screen coordinates taking
    /// camera offset and window dimensions into accout.
    Vector2dT computeTargetPosition() const;

    /// Position in screen coordinates
    Vector2dT m_position;

    /// A texture object
    SDL_Texture* m_texture;

    /// Source rect in the texture
    SDL_Rect m_sourceRect;

    /// Position on the target renderer
    SDL_Rect m_targetRect;

    /// Rendering offset of the tile set. Used when
    /// moving the camera
    Vector2dT m_offset;

    /// Rendering offset to accout for window dimensions.
    /// Used to keep camera viewport centered.
    Vector2dT m_windowOffset;

    /// Flipping action
    SDL_RendererFlip m_flip;

    /// Angle in degrees that indicates the flip rotation
    double m_flip_angle;
};

}  // namespace engine
}  // namespace ctb

#endif  // ENGINE_GRAPHIC_TEXTUREBASEDRENDERABLE_HPP
