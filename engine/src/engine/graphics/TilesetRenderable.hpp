// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_GRAPHIC_TILESET_HPP
#define ENGINE_GRAPHIC_TILESET_HPP

#include <memory>

#include <parser/DynamicTilestore.hpp>
#include <parser/TilesetConfig.hpp>

#include "engine/graphics/TextureBasedRenderable.hpp"

namespace ctb {
namespace parser {
class LevelConfig;
}
class DynamicTilestore;

namespace engine {

/// Class that represents a tileset.
class TilesetRenderable : public TextureBasedRenderable {
   public:
    /// \brief Constructor
    ///
    /// \param config Tileset configuration
    /// \param levelConfig Level configuration
    /// \param flip Flip level?
    TilesetRenderable(parser::TilesetConfig& config,
                      parser::LevelConfig* levelConfig,
                      bool flip = false);

    /// Renders the tileset
    void render() override;

    /// Returns the width of the tileset
    int width() const override;

    /// Retruns the height of the tileset
    int height() const override;

    /// Return the tilestore
    DynamicTilestore* tiles() { return m_tiles.get(); }

    /// Destructor
    ~TilesetRenderable() override = default;

   protected:
    /// With of a tile
    int m_tileWidth;

    /// Height of a tile
    int m_tileHeight;

    /// Distance between tiles on tilesheet
    int m_tileOffset;

    /// Tiles per row in tiles
    int m_tilesPerRow;

    /// Width of level in tiles
    int m_levelWidth;

    /// Height of level in tiles
    int m_levelHeight;

    /// Tilestore
    std::unique_ptr<DynamicTilestore> m_tiles;
};

}  // namespace engine
}  // namespace ctb

#endif  // ENGINE_GRAPHIC_TILESET_HPP
