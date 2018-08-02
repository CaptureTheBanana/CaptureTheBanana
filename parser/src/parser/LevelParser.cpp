// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "parser/LevelParser.hpp"
#include "parser/BackgroundConfig.hpp"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <common/Utils.hpp>

namespace ctb {
namespace parser {

LevelConfig* parseLevel(const std::string& absoluteFilename, const std::string& relativeFilename) {
    namespace cutils = ctb::common::utils;

    // Read property tree from xml file
    using boost::property_tree::ptree;
    ptree pt;
    read_xml(absoluteFilename, pt);

    if (!pt.get_child_optional("level")) {
        throw std::invalid_argument("the file was is not a level file!");
    }

    std::string absolutePath = cutils::getDirectoryPathWithSlash(absoluteFilename);
    LevelConfig* levelConfig = new LevelConfig();
    levelConfig->setLevelFilename(relativeFilename);
    levelConfig->setAbsolutePath(absolutePath);

    // get dimensions
    levelConfig->setWidth(pt.get("level.width", 0));
    levelConfig->setHeight(pt.get("level.height", 0));

    // get gravitation
    levelConfig->setGravitation(pt.get<float>("level.gravitation", 20.0f));

    // get type
    std::string type = pt.get<std::string>("level.<xmlattr>.type");
    if (type == "CENTER") {
        levelConfig->setLevelType(LevelType::CENTER);
    } else if (type == "DEFAULT") {
        levelConfig->setLevelType(LevelType::DEFAULT);
    } else if (type == "END") {
        levelConfig->setLevelType(LevelType::END);
    }

    // Parse all children of the level tag
    for (const ptree::value_type& v : pt.get_child("level")) {
        if (v.first == "tileset") {
            int layer = v.second.get<int>("layer", 0);
            float friction = v.second.get<float>("friction", 0.5);
            bool collideBots = v.second.get<bool>("collideBots", false);
            bool collidePlayers = v.second.get<bool>("collidePlayers", false);
            bool collideProjectiles = v.second.get<bool>("collideProjectiles", false);
            std::string tileArrangement = v.second.get<std::string>("tileArrangement", "");

            TilesetConfig tileset(layer, friction, collideBots, collideProjectiles, collidePlayers,
                                  tileArrangement);

            levelConfig->addTileset(tileset);
        }

        if (v.first == "tilesheet") {
            // Get path from given file name
            std::string tilesetFilename = v.second.get<std::string>("filename", "");
            int tileWidth = v.second.get<int>("tileWidth", 0);
            int tileHeight = v.second.get<int>("tileHeight", 0);
            int tileOffset = v.second.get<int>("tileOffset", 0);
            int tilesPerRow = v.second.get<int>("tilesPerRow", 0);
            int maxTileId = v.second.get<int>("maxTileId", 0);

            levelConfig->setTilesheetFile(tilesetFilename);
            levelConfig->setTileWidth(tileWidth);
            levelConfig->setTileHeight(tileHeight);
            levelConfig->setTileOffset(tileOffset);
            levelConfig->setTilesPerRow(tilesPerRow);
            levelConfig->setMaxTileId(maxTileId);
        }

        if (v.first == "background") {
            int layer = v.second.get<int>("layer", 0);
            int scrollSpeed = v.second.get<int>("scrollSpeed", 100);
            std::string imageFilename = v.second.get<std::string>("image", "");

            BackgroundConfig background(layer, scrollSpeed, imageFilename, absolutePath);

            levelConfig->addBackground(background);
        }

        if (v.first == "doorL") {
            int x = v.second.get<int>("x", 0);
            int y = v.second.get<int>("y", 0);

            levelConfig->setDoorL(x, y);
        }

        if (v.first == "doorR") {
            int x = v.second.get<int>("x", 0);
            int y = v.second.get<int>("y", 0);

            levelConfig->setDoorR(x, y);
        }

        if (v.first == "weaponSpawns") {
            for (const ptree::value_type& w : v.second) {
                if (w.first == "spawn") {
                    int x = w.second.get<int>("x", 0);
                    int y = w.second.get<int>("y", 0);
                    std::string spawnType = w.second.get<std::string>("type", "");

                    TypeSpawn spawn(x, y, spawnType);

                    levelConfig->addWeaponSpawn(spawn);
                }
            }
        }

        if (v.first == "botSpawns") {
            for (const ptree::value_type& w : v.second) {
                if (w.first == "spawn") {
                    int x = w.second.get<int>("x", 0);
                    int y = w.second.get<int>("y", 0);
                    std::string spawnType = w.second.get<std::string>("type", "");

                    TypeSpawn spawn(x, y, spawnType);

                    levelConfig->addBotSpawn(spawn);
                }
            }
        }

        if (v.first == "playerSpawns") {
            for (const ptree::value_type& w : v.second) {
                if (w.first == "spawn") {
                    int x = w.second.get<int>("x", 0);
                    int y = w.second.get<int>("y", 0);

                    levelConfig->setPlayerSpawn(x, y);
                }
            }
        }

        if (v.first == "flagSpawn") {
            int x = v.second.get<int>("x", 0);
            int y = v.second.get<int>("y", 0);

            levelConfig->setFlagSpawn(x, y);
        }
    }

    return levelConfig;
}

void saveLevel(gsl::not_null<LevelConfig*> levelConfig, const std::string& gameXmlFile) {
    using boost::property_tree::ptree;
    using boost::property_tree::xml_writer_make_settings;
    namespace cutils = ctb::common::utils;

    ptree pt;
    if (levelConfig->getLevelType() == LevelType::CENTER) {
        pt.put("level.<xmlattr>.type", "CENTER");
    } else if (levelConfig->getLevelType() == LevelType::DEFAULT) {
        pt.put("level.<xmlattr>.type", "DEFAULT");
    } else if (levelConfig->getLevelType() == LevelType::END) {
        pt.put("level.<xmlattr>.type", "END");
    }

    pt.put("level.gravitation", levelConfig->getGravitation());

    // write left door
    pt.put("level.doorL.x", levelConfig->getDoorL().x);
    pt.put("level.doorL.y", levelConfig->getDoorL().y);

    // write right door
    pt.put("level.doorR.x", levelConfig->getDoorR().x);
    pt.put("level.doorR.y", levelConfig->getDoorR().y);

    // write flag spawn
    pt.put("level.flagSpawn.x", levelConfig->getFlagSpawn().x);
    pt.put("level.flagSpawn.y", levelConfig->getFlagSpawn().y);

    // save playerspawns
    for (size_t x = 0; x < levelConfig->getPlayerSpawns().size(); x++) {
        for (size_t y = 0; y < levelConfig->getPlayerSpawns()[x].size(); y++) {
            ptree playerSpawn;
            playerSpawn.put("x", x);
            playerSpawn.put("y", levelConfig->getPlayerSpawns()[x][y]);

            pt.add_child("level.playerSpawns.spawn", playerSpawn);
        }
    }

    // save weapon spawns
    for (TypeSpawn& spawn : levelConfig->getWeaponSpawns()) {
        ptree weaponSpawn;
        weaponSpawn.put("x", spawn.getPosition().x);
        weaponSpawn.put("y", spawn.getPosition().y);
        weaponSpawn.put("type", spawn.getType());

        pt.add_child("level.weaponSpawns.spawn", weaponSpawn);
    }

    // save bot spawns
    for (TypeSpawn& spawn : levelConfig->getBotSpawns()) {
        ptree botSpawn;
        botSpawn.put("x", spawn.getPosition().x);
        botSpawn.put("y", spawn.getPosition().y);
        botSpawn.put("type", spawn.getType());

        pt.add_child("level.botSpawns.spawn", botSpawn);
    }

    // save backgrounds
    for (BackgroundConfig& backgroundConfig : levelConfig->getBackgrounds()) {
        ptree background;
        background.put("layer", backgroundConfig.getLayer());
        background.put("scrollSpeed", backgroundConfig.getScrollSpeed());
        background.put("image", backgroundConfig.getRelativeImageFilename());

        pt.add_child("level.background", background);
    }

    // save layers
    for (TilesetConfig& tilesetConfig : levelConfig->getTilesets()) {
        ptree tileset;
        tileset.put("layer", tilesetConfig.getLayer());

        tileset.put("collideBots", tilesetConfig.canCollideBots());
        tileset.put("collidePlayers", tilesetConfig.canCollidePlayers());
        tileset.put("collideProjectiles", tilesetConfig.canCollideProjectiles());

        tileset.put("tileArrangement", tilesetConfig.getTileArrangement());

        tileset.put("friction", tilesetConfig.getFriction());

        pt.add_child("level.tileset", tileset);
    }

    // write dimensions
    pt.put("level.width", levelConfig->getWidth());
    pt.put("level.height", levelConfig->getHeight());

    // save filese information
    pt.put("level.tilesheet.filename", levelConfig->getRelativeTilesheetFilename());
    pt.put("level.tilesheet.tileWidth", levelConfig->getTileWidth());
    pt.put("level.tilesheet.tileHeight", levelConfig->getTileHeight());
    pt.put("level.tilesheet.tileOffset", levelConfig->getTileOffset());
    pt.put("level.tilesheet.tilesPerRow", levelConfig->getTilesPerRow());
    pt.put("level.tilesheet.maxTileId", levelConfig->getMaxTileId());

    write_xml(cutils::getDirectoryPathWithSlash(gameXmlFile) + levelConfig->getLevelFilename(), pt,
              std::locale(), xml_writer_make_settings<std::string>(' ', 4));
}

}  // namespace parser
}  // namespace ctb
