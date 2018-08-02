// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_GRAPHIC_SDL_RENDERALBE_HPP
#define ENGINE_GRAPHIC_SDL_RENDERALBE_HPP

#include "engine/Object.hpp"

namespace ctb {
namespace engine {

// Window forward declaration
class IWindowWithEngine;

/// Interface definition for all renderable objects
class SDLRenderable : public Object {
   public:
    SDLRenderable() = default;

    /// Render method
    virtual void render() = 0;

    ~SDLRenderable() override = default;
};

}  // namespace engine
}  // namespace ctb

#endif
