// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_GRAPHIC_BACKGROUND_HPP
#define ENGINE_GRAPHIC_BACKGROUND_HPP

#include <parser/BackgroundConfig.hpp>

#include "engine/graphics/TextureBasedRenderable.hpp"

namespace ctb {
namespace engine {

/// Class responsible for rendering backgrounds for the paralax effect
class Background : public TextureBasedRenderable {
   public:
    /// \brief creates new background layer
    ///
    /// \param backgroundConf config of background
    /// \param levelWidth width of level in pixels
    /// \param levelHeight height of level in pixels
    /// \throws std::invalid_argument if image of backgroundConf is invalid
    Background(parser::BackgroundConfig& backgroundConf, int levelWidth, int levelHeight);

    /// \brief Renders the background
    void render() override;

    /// \brief Destructor
    ~Background() override = default;

   private:
    // scroll speed of background layer
    float m_scrollSpeed;

    /// width of level in pixels
    int m_levelWidth;

    /// height of level in pixels
    int m_levelHeight;
};

}  // namespace engine
}  // namespace ctb

#endif
