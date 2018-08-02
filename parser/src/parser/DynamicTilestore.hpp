// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef PARSER_DYNAMICTILESTORE_HPP
#define PARSER_DYNAMICTILESTORE_HPP

#include <string>
#include <vector>

namespace ctb {

/// \brief a 2d representation of tiles
class DynamicTilestore {
   public:
    /// \param width width of tile array
    /// \param height height of tile array
    /// \param tileWidth width of tiles
    /// \param tileHeight height of tiles
    DynamicTilestore(int width, int height, int tileWidth, int tileHeight);

    /// \brief builds TileArray from string
    ///
    /// \param tileString tileArrangement from TilesetConfig: formatted like this: 1 2 31 2 1 0 0 0
    /// 9
    /// \param width width of tile array
    /// \param height height of tile array
    /// \param tileWidth width of tiles
    /// \param tileHeight height of tiles
    DynamicTilestore(const std::string& tileString,
                     int width,
                     int height,
                     int tileWidth,
                     int tileHeight);

    /// \brief delivers tile id at x, y
    ///
    /// \param x coordinate of tile
    /// \param y coordinate of tile
    /// \throws out_of_range if x or y is out of bounds
    int get(int x, int y) const;

    /// \brief inserts tile to tiles
    ///
    /// \param x x-coordinate
    /// \param y y-coordinate
    /// \param id id to be set to x an y
    /// \throws out_of_range if x or y is out of bounds
    void set(int x, int y, int id);

    /// \brief sets the width of the tilestore
    ///
    /// \param width new width
    void setWidth(int width);

    /// \brief increases the width of the tilestore by 1
    void increaseWidth();

    /// \brief decreases the width of the tilestore by 1
    void decreaseWidth();

    /// \brief delivers the width of this tileset
    ///
    /// \return int width of full tilestore in tiles
    int getWidth();

    /// \brief sets the height of this tileset
    void setHeight(int height);

    /// \brief increases the height of the tilestore by 1
    void increaseHeight();

    /// \brief decreases the height of the tilestore by 1
    void decreaseHeight();

    /// \brief delivers the height of this tileset
    ///
    /// \return int height of full tilestore in tiles
    int getHeight();

    /// \brief delivers the height of one tile
    ///
    /// \return int height of one tile
    int getTileHeight();

    /// \brief delivers the width of one tile
    ///
    /// \return int width of one tile
    int getTileWidth();

    /// \brief flips this tilestore horizontaly
    ///         1 0 4 1 5       5 1 4 0 1
    ///         4 3 2 1 7   =>  7 1 2 3 4
    void flip();

    /// \brief converts this tile array into a tileString
    ///
    /// \return tileString that can be added to the TilesetConfig to be stored into a xml file
    std::string toTileString();

    /// Destructor
    virtual ~DynamicTilestore() = default;

   private:
    /// tile ids indexed by x and y.
    /// m_tiles[x][y] = tile id
    /// 0 means no tile
    std::vector<std::vector<int>> m_tiles;

    /// width of tile array
    int m_width{0};

    /// height of tile array
    int m_height{0};

    /// width of tiles
    int m_tileWidth;

    /// height of tiles
    int m_tileHeight;
};

}  // namespace ctb

#endif  // PARSER_DYNAMICTILESTORE_HPP
