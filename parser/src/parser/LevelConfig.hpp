// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef PARSER_LEVELCONFIG_HPP
#define PARSER_LEVELCONFIG_HPP

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <common/Vector2d.hpp>

#include "parser/BackgroundConfig.hpp"
#include "parser/TilesetConfig.hpp"

namespace ctb {
namespace parser {

/// \brief Type of level:
///         CENTER level is always in the middle of a game ans schould be symmetrical
///         DEFAULT levels are in between CENTER and END levels
///         END levels are at the end of the world
enum class LevelType { CENTER, DEFAULT, END };

/// \brief describes spawns of bots or weapons
class TypeSpawn {
   public:
    /// \param x x-coordinate
    /// \param y y-coordinate
    /// \param type additional information like bottype(eagle, warrior, ...), or weapontype (sword,
    /// sniper, ...)
    TypeSpawn(int x, int y, std::string type) : m_position(x, y), m_type(std::move(type)) {}

    /// \brief sets the position of the spawn
    ///
    /// \param x x-coordinate
    /// \param y y-coordinate
    inline void setPosition(int x, int y) {
        m_position.x = x;
        m_position.y = y;
    }

    /// \brief delivers the position of this spawn
    ///
    /// \return common::Vector2d& spawn position in an vector
    inline common::Vector2d& getPosition() { return m_position; }

    /// \brief sets the type of elements that can spawn here
    ///
    /// \param type additional information like bottype, or weapontype
    inline void setType(std::string type) { m_type = type; }

    /// \brief delivers the type of this spawn
    ///
    /// \return std::string type of this as a string
    inline std::string getType() { return m_type; }

    virtual ~TypeSpawn() = default;

   private:
    /// position of the spawn
    common::Vector2d m_position;
    /// type of the weapon or bot that spawns (e.g. "Eagle", "Warrior" or "Sniper"
    std::string m_type;
};

/// \brief stores configuration of the level
class LevelConfig {
   public:
    LevelConfig()
        : m_doorL(-1, -1),
          m_doorR(-1, -1),
          m_flagSpawn(-1, -1),
          m_teamspawnL(-1, -1),
          m_teamspawnR(-1, -1) {}

    /// \brief adds a layer to the config
    ///         takes ownership of the layer
    ///
    /// \param TilesetConfig to be added
    inline void addTileset(TilesetConfig& tileset) { m_tilesets.push_back(tileset); }

    /// \brief delivers vector with all layers
    ///
    /// \return std::vector<LayerConfig>& reference to vector with pointers to all layers
    inline std::vector<TilesetConfig>& getTilesets() { return m_tilesets; }

    /// \brief adds a background layer to the config
    ///         takes the ownership of the layer
    ///
    /// \param background to be added
    inline void addBackground(const BackgroundConfig& background) {
        m_backgrounds.push_back(background);
    }

    /// \brief delivers vector with all backgrounds
    ///
    /// \return std::vector<BackgroundConfig>& reference to a vector with pointers to all
    /// backgrounds
    inline std::vector<BackgroundConfig>& getBackgrounds() { return m_backgrounds; }

    /// \brief sets type of this level
    ///
    /// \param levelType new type of level
    inline void setLevelType(LevelType levelType) { m_type = levelType; }

    /// \brief delivers type of this level
    ///
    /// \return LevelType type of this level
    inline LevelType getLevelType() { return m_type; }

    void flipPoints() {
        // doors
        common::Vector2d tmp = m_doorL;
        m_doorL = m_doorR;
        m_doorR = tmp;
        m_doorL.x = m_width - m_doorL.x - 1;
        m_doorR.x = m_width - m_doorR.x - 1;

        // flag spawn
        m_flagSpawn.x = m_width - m_flagSpawn.x - 1;

        // player spawns
        std::reverse(m_playerSpawns.begin(), m_playerSpawns.end());

        // bot spawns
        for (size_t i = 0; i < m_botSpawns.size(); i++) {
            auto& pos = m_botSpawns[i].getPosition();
            pos.x = m_width - pos.x - 1;
        }

        // weapon spawns
        for (size_t i = 0; i < m_weaponSpawns.size(); i++) {
            auto& pos = m_weaponSpawns[i].getPosition();
            pos.x = m_width - pos.x - 1;
        }
    }

    /// \brief sets left door position
    ///
    /// \param x x-position
    /// \param y y-position
    inline void setDoorL(int x, int y) {
        m_doorL.x = x;
        m_doorL.y = y;
    }

    /// \brief delivers position of left door
    ///
    /// \return common::Vector2d& reference to the location of the left door
    inline common::Vector2d& getDoorL() { return m_doorL; }

    /// \brief sets right door position
    ///
    /// \param x x-position
    /// \param y y-position
    inline void setDoorR(int x, int y) {
        m_doorR.x = x;
        m_doorR.y = y;
    }

    /// \brief delivers position of right door
    ///
    /// \return common::Vector2d& reference to the location of the right door
    inline common::Vector2d& getDoorR() { return m_doorR; }

    /// \brief sets flag spawn position
    ///
    /// \param x x-position
    /// \param y y-position
    inline void setFlagSpawn(int x, int y) {
        m_flagSpawn.x = x;
        m_flagSpawn.y = y;
    }

    /// \brief delivers position of flag spawn
    ///
    /// \return common::Vector2d& reference to the location of the flag spawn
    inline common::Vector2d& getFlagSpawn() { return m_flagSpawn; }

    /// util function for tile to world coordinates
    common::Vector2d toWorld(common::Vector2d pos) {
        pos.x *= m_tileWidth;
        pos.y *= m_tileWidth;
        return pos;
    }

    /// \brief sets player Spawn
    ///         there is a maximum of one player spawns per x coordinate
    ///         the coordinates are measured in tiles
    ///
    /// \param x x-position
    /// \param y y-position
    /// \throws out_of_range if x is out side the level
    inline void setPlayerSpawn(int x, int y) {
        if (x < 0 || y < 0 || static_cast<size_t>(x) >= m_playerSpawns.size()) {
            throw std::out_of_range("the player spawn location cannot be outside the level");
        }

        // get y coordinates for x position
        std::vector<uint32_t>& yCoordinates = m_playerSpawns.at(static_cast<size_t>(x));
        for (size_t i = 0; i < yCoordinates.size(); i++) {
            if (yCoordinates[i] == static_cast<uint32_t>(y)) {
                return;
            }
        }

        yCoordinates.push_back(static_cast<uint32_t>(y));
    }

    /// \brief removes Player Spawn
    ///
    /// \param x x-position at which the spawn should be deleted
    /// \param y y-position at which the spawn should be deleted
    /// \throws out_of_range if spawn isnt set
    inline void removePlayerSpawn(int x, int y) {
        if (x < 0) {
            throw std::out_of_range("x must not be negative!");
        }

        std::vector<uint32_t>& yCoordinates = m_playerSpawns.at(static_cast<size_t>(x));
        for (size_t i = 0; i < yCoordinates.size(); i++) {
            if (yCoordinates[i] == static_cast<uint32_t>(y)) {
                yCoordinates.erase(yCoordinates.begin() + static_cast<ptrdiff_t>(i));
                return;
            }
        }

        throw std::out_of_range("this player spawn doesnt exist");
    }

    /// \brief delivers player spawns for all x coordinates
    ///         there is a maximum of one player spawns per x coordinate
    ///         the coordinates are measured in tiles
    ///         if there is no player spawn, it returns a nullptr
    ///
    /// \return std::vector<uint32_t>& vector with spawn y coordinates in it: spawns[x] = {y1, y2,
    /// y3}
    inline std::vector<std::vector<uint32_t>>& getPlayerSpawns() { return m_playerSpawns; }

    /// \brief adds bot spawn to this level config
    ///
    /// \param spawn spawn to be added
    inline void addBotSpawn(TypeSpawn& spawn) { m_botSpawns.push_back(spawn); }

    /// \brief delivers spawn positions for bots
    ///
    /// \return std::vector<TypeSpawn>& vector with pointers to all bot spawns inside
    inline std::vector<TypeSpawn>& getBotSpawns() { return m_botSpawns; }

    /// \brief adds weapon spawn to this level config
    ///
    /// \param spawn spawn to be added
    inline void addWeaponSpawn(TypeSpawn spawn) { m_weaponSpawns.push_back(spawn); }

    /// \brief delivers spawn positions for weapons
    ///
    /// \return std::vector<TypeSpawn*>& vector with pointers to all weapon spawns inside
    inline std::vector<TypeSpawn>& getWeaponSpawns() { return m_weaponSpawns; }

    /// \brief sets gravitation
    ///
    /// \param gravitation new gravitation
    inline void setGravitation(float gravitation) {
        if (gravitation < 3.0f) {
            throw std::invalid_argument("Expects gravitation >= 3.0");
        }
        m_gravitation = gravitation;
    }

    /// \brief delivers gravity
    ///
    /// \return float gravitation of this level
    inline float getGravitation() { return m_gravitation; }

    /// \brief sets tileset file
    ///
    /// \param filename new tileset filename
    inline void setTilesheetFile(std::string filename) { m_tilesheetFilename = filename; }

    /// \brief delivers filename of tileset
    ///
    /// \return std::string filename of tileset
    inline std::string getTilesheetFilename() { return m_absolutePath + m_tilesheetFilename; }

    /// \brief delivers filename of tileset
    ///
    /// \return std::string filename of tileset
    inline std::string getRelativeTilesheetFilename() { return m_tilesheetFilename; }

    /// \brief sets the width of tiles
    ///
    /// \param tileWidth new width
    inline void setTileWidth(int tileWidth) { m_tileWidth = tileWidth; }

    /// \brief delivers width of tiles
    ///
    /// \return int width of tiles
    inline int getTileWidth() { return m_tileWidth; }

    /// \brief sets the height of tiles
    ///
    /// \param tileHeight new height
    inline void setTileHeight(int tileHeight) { m_tileHeight = tileHeight; }

    /// \brief delivers height of tiles
    ///
    /// \return int htight of tiles
    inline int getTileHeight() { return m_tileHeight; }

    /// \brief sets the offset between tiles
    ///
    /// \param tileOffset new offset
    inline void setTileOffset(int tileOffset) { m_tileOffset = tileOffset; }

    /// \brief delivers offset between tiles in the tilesheet
    ///
    /// \return int offset between tiles
    inline int getTileOffset() { return m_tileOffset; }

    /// \brief sets the amount of tiles per row
    ///
    /// \param tilesPerRow new amount of tiles per row
    inline void setTilesPerRow(int tilesPerRow) { m_tilesPerRow = tilesPerRow; }

    /// \brief delivers amount of tiles per row
    ///
    /// \return int tiles per row
    inline int getTilesPerRow() { return m_tilesPerRow; }

    /// \brief sets the max id of tiles
    ///
    /// \param maxTileId new max id
    inline void setMaxTileId(int maxTileId) { m_maxTileId = maxTileId; }

    /// \brief delivers the max id of tiles
    ///
    /// \return int max id
    inline int getMaxTileId() { return m_maxTileId; }

    /// \brief sets width of level
    ///
    /// \param width new width
    inline void setWidth(int width) {
        if (width < 0) {
            throw std::out_of_range("size cannot be negative");
        }
        m_playerSpawns.resize(static_cast<unsigned>(width));
        m_width = width;
    }

    /// \brief delivers width ov level in tiles
    ///
    /// \return int width
    inline int getWidth() { return m_width; }

    /// get width in pixels
    inline int getPixelWidth() { return m_width * m_tileWidth; }

    /// \brief sets height of level
    ///
    /// \param width new height
    inline void setHeight(int height) { m_height = height; }

    /// \brief delivers height ov level in tiles
    ///
    /// \return int height
    inline int getHeight() { return m_height; }

    /// get height in pixels
    inline int getPixelHeight() { return m_height * m_tileHeight; }

    /// \brief sets the relative level filename from game file
    ///
    /// \param filename level filename
    inline void setLevelFilename(std::string filename) { m_levelFilename = std::move(filename); }

    /// \brief gets the level filename
    ///
    /// \return relative filename from game file
    inline std::string getLevelFilename() { return m_levelFilename; }

    /// \brief sets the absolute path to parent directory of level.xml
    ///         ignores absolute paths of backgroundConfigs
    ///
    ///         only used in LevelParser
    ///         urelevant for the GameEditor
    ///
    ///
    /// \param new path
    inline void setAbsolutePath(std::string path) { m_absolutePath = path; }

    virtual ~LevelConfig() {}

   private:
    /// foreground layers of level
    std::vector<TilesetConfig> m_tilesets;

    // background layers of level
    std::vector<BackgroundConfig> m_backgrounds;

    /// holds type of level
    LevelType m_type{LevelType::DEFAULT};

    /// defines the door locations
    common::Vector2d m_doorL;
    common::Vector2d m_doorR;

    /// location where the flag gets spawned at the beginning or if all Players are dead
    common::Vector2d m_flagSpawn;

    /// spawn location of the teams (gets used on start or if all players die)
    common::Vector2d m_teamspawnL;
    common::Vector2d m_teamspawnR;

    /// spawn location of players (players spawn here if they die)
    /// the spawns are indexed by their x coordinate: m_playerSpawns[3] delivers the y coordinates
    /// (or NO_PLAYER_SPAWN for no spawn)
    std::vector<std::vector<uint32_t>> m_playerSpawns;

    /// spawn locations of bots
    std::vector<TypeSpawn> m_botSpawns;

    /// spawn locations of weapons
    std::vector<TypeSpawn> m_weaponSpawns;

    /// gravitation of this level
    float m_gravitation{20.0f};

    /// path of tilesheet image
    std::string m_tilesheetFilename;
    /// width of tiles
    int m_tileWidth{0};
    // height of tiles
    int m_tileHeight{0};
    /// distance between tiles on tilesheet
    int m_tileOffset{0};
    /// width of level in tiles
    int m_width{0};
    /// height of level in tiles
    int m_height{0};
    /// tiles per row in tiles
    int m_tilesPerRow{0};
    /// max id of tiles
    int m_maxTileId{0};

    /// relative filename from game.xml to level.xml
    std::string m_levelFilename{};

    /// absolute path to parent directory of level.xml
    /// doesnt need to be changed
    std::string m_absolutePath{""};
};

}  // namespace parser
}  // namespace ctb

#endif
