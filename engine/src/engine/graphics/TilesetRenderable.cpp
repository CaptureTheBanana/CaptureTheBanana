// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <SDL.h>
#include <SDL_image.h>
#include <parser/LevelConfig.hpp>

#include "engine/Window.hpp"
#include "engine/graphics/TilesetRenderable.hpp"

namespace ctb {
namespace engine {

namespace {
using ctb::parser::LevelConfig;
using ctb::parser::TilesetConfig;
}  // namespace

TilesetRenderable::TilesetRenderable(parser::TilesetConfig& config,
                                     parser::LevelConfig* levelConfig,
                                     bool flip)
    : TextureBasedRenderable(Window::getWindow().loadTexture(levelConfig->getTilesheetFilename())) {
    m_tileWidth = levelConfig->getTileWidth();
    m_tileHeight = levelConfig->getTileHeight();
    m_tileOffset = levelConfig->getTileOffset();
    m_tilesPerRow = levelConfig->getTilesPerRow();
    m_levelWidth = levelConfig->getWidth();
    m_levelHeight = levelConfig->getHeight();
    m_tiles = std::make_unique<DynamicTilestore>(config.getTileArrangement(), m_levelWidth,
                                                 m_levelHeight, m_tileWidth, m_tileHeight);

    if (flip) {
        m_tiles->flip();
    }
    m_flip = flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
}

int TilesetRenderable::width() const {
    return m_levelWidth * m_tileWidth;
}

int TilesetRenderable::height() const {
    return m_tileHeight * m_levelHeight;
}

void TilesetRenderable::render() {
    // TODO(felix): Render only visible area.

    SDL_Rect srcRect = {0, 0, m_tileWidth, m_tileHeight};
    SDL_Rect dstRect = {0, 0, m_tileWidth, m_tileHeight};
    for (int y = 0; y < m_levelHeight; ++y) {
        for (int x = 0; x < m_levelWidth; ++x) {
            int tile = m_tiles->get(x, y) - 1;
            if (tile <= -1) {
                continue;
            }

            int tile_x = tile % m_tilesPerRow;
            int tile_y = tile / m_tilesPerRow;

            srcRect.x = (tile_x * m_tileWidth) + (tile_x * m_tileOffset);
            srcRect.y = (tile_y * m_tileHeight) + (tile_y * m_tileOffset);
            dstRect.x = x * m_tileWidth - m_offset.x + m_windowOffset.x;
            dstRect.y = y * m_tileHeight - m_offset.y + m_windowOffset.y;
            SDL_RenderCopyEx(Window::getWindow().renderer(), m_texture, &srcRect, &dstRect, 0,
                             nullptr, m_flip);
        }
    }
}

}  // namespace engine
}  // namespace ctb
