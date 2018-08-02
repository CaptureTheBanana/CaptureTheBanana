// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "engine/Window.hpp"
#include "engine/graphics/Background.hpp"

namespace ctb {
namespace engine {

Background::Background(parser::BackgroundConfig& backgroundConf, int levelWidth, int levelHeight)
    : TextureBasedRenderable(Window::getWindow().loadTexture(backgroundConf.getImageFilename())),
      m_levelWidth(levelWidth),
      m_levelHeight(levelHeight) {
    // calculate scroll speed
    m_scrollSpeed = static_cast<float>(backgroundConf.getScrollSpeed()) / 100.0F;

    // get width and height of texture
    int textureWidth, textureHeight;
    SDL_QueryTexture(m_texture, nullptr, nullptr, &textureWidth, &textureHeight);

    // calculate ingame background size
    m_targetRect.h = m_levelHeight;
    m_targetRect.w = (m_levelHeight * textureWidth) / textureHeight;
}

void Background::render() {
    // repeat image to the end of the level
    for (int startX = 0; startX < m_levelWidth; startX += m_targetRect.w) {
        // calculate x and y
        m_targetRect.x =
            startX -
            static_cast<int>(static_cast<float>(m_offset.x - (Window::getWindow().w() / 2)) *
                             m_scrollSpeed);
        m_targetRect.y =
            0 - static_cast<int>(static_cast<float>(m_offset.y - (Window::getWindow().h() / 2)) *
                                 m_scrollSpeed);

        SDL_RenderCopy(Window::getWindow().renderer(), m_texture, nullptr, &m_targetRect);
    }
}

}  // namespace engine
}  // namespace ctb
