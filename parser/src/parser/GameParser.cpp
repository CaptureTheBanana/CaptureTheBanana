// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <vector>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <common/Utils.hpp>

#include "parser/GameParser.hpp"
#include "parser/LevelConfig.hpp"
#include "parser/LevelParser.hpp"

namespace ctb {
namespace parser {
namespace {
using boost::filesystem::path;
using boost::property_tree::ptree;
using boost::property_tree::xml_writer_make_settings;
namespace cutils = ctb::common::utils;
}  // namespace

GameConfig* parseGame(const std::string& file) {
    GameConfig* config = new GameConfig();
    std::vector<LevelConfig*> m_levels;
    ptree pt;
    read_xml(file, pt);
    path p(file);
    std::string dir = p.parent_path().string() + cutils::CTB_SEPARATOR;

    /// Parse all children of the game tag

    // folders
    config->setFontsFolder(dir + pt.get("game.fonts.<xmlattr>.folder", ""));
    config->setSoundsFolder(dir + pt.get("game.sounds.<xmlattr>.folder", ""));
    config->setPlayerLayer(pt.get("game.playerLayer.<xmlattr>.layer", 0));
    config->setFlagFilename(dir + pt.get("game.flag.<xmlattr>.file", ""));
    config->setInputsFile(dir + pt.get("game.inputs.<xmlattr>.file", ""));
    config->setDoorFilename(dir + pt.get("game.door.<xmlattr>.file", ""));

    // offsets
    config->setPlayerOffset(pt.get("game.offsets.player", 0));
    config->setDoorOffset(pt.get("game.offsets.door", 0));

    // children
    for (const ptree::value_type& lvl : pt.get_child("game.levels")) {
        std::string filename = lvl.second.get<std::string>("<xmlattr>.file", "");
        LevelConfig* level = parseLevel(dir + filename, filename);
        config->getLevels().insert(std::make_pair(level->getLevelType(), level));
    }
    for (const ptree::value_type& player : pt.get_child("game.players")) {
        std::string filename = dir + player.second.get("filename", "");
        int frameWidth = player.second.get("frameWidth", 0);
        int frameHeight = player.second.get("frameHeight", 0);
        int numFrames = player.second.get("numFrames", 0);
        config->getPlayers().push_back(
            new PlayerConfig(filename, frameWidth, frameHeight, numFrames));
    }
    for (const ptree::value_type& bot : pt.get_child("game.bots")) {
        BotConfig* bc = new BotConfig();
        bc->setFilename(dir + bot.second.get("<xmlattr>.filename", ""));
        bc->setName(bot.second.get("name", ""));
        bc->setNumFrames(bot.second.get<int>("num_frames", 0));
        bc->setFrameWidth(bot.second.get<int>("frame_width", 0));
        bc->setFrameHeight(bot.second.get<int>("frame_height", 0));
        config->getBots().push_back(bc);
    }
    for (const ptree::value_type& weapon : pt.get_child("game.weapons")) {
        std::string filename = dir + weapon.second.get("<xmlattr>.filename", "");
        std::string name = weapon.second.get("name", "");
        float range = weapon.second.get<float>("range", 0.0);
        float as = weapon.second.get<float>("attackspeed", 0.0);
        float damage = weapon.second.get<float>("damage", 0.0);
        std::string special = weapon.second.get("special", "");
        std::string projectile = weapon.second.get("projectile", "");
        config->getWeapons().push_back(
            new WeaponConfig(filename, name, range, as, damage, special, projectile));
    }
    for (const ptree::value_type& proj : pt.get_child("game.projectiles")) {
        std::string name = proj.second.get("<xmlattr>.name", "");
        std::string sprite = dir + proj.second.get("sprite", "");
        config->getProjectiles().push_back(new ProjectileConfig(sprite, name));
    }
    return config;
}

void saveGameLevels(const std::string& gamexml, gsl::not_null<GameConfig*> game) {
    ptree pt;
    read_xml(gamexml, pt, boost::property_tree::xml_parser::trim_whitespace);

    try {
        pt.get_child("game.levels");

        // Remove levels node
        pt.get_child("game").erase("levels");
    } catch (boost::exception&) {
        // There is nothing to remove
    }

    // Add all levels to the xml file
    ptree levels;
    for (auto& lvl : game->getLevels()) {
        ptree level;
        level.put("<xmlattr>.file", lvl.second->getLevelFilename());
        levels.add_child("level", level);
    }
    pt.get_child("game").add_child("levels", levels);

    // Pretty save gamle.xml
    boost::property_tree::write_xml(gamexml, pt, std::locale(),
                                    xml_writer_make_settings<std::string>(' ', 4));
}

std::string to_string(GameValidatorStatus status) {
    switch (status) {
        case GameValidatorStatus::kOk:
            return "Ok";
        case GameValidatorStatus::kEmptyGame:
            return "Game has no levels.";
        case GameValidatorStatus::kEmptyLevel:
            return "Level has no layers/tilesets.";
        case GameValidatorStatus::kNoDoorL:
            return "No left door available.";
        case GameValidatorStatus::kNoDoorR:
            return "No right door available.";
        case GameValidatorStatus::kNoFlagSpawn:
            return "No flag spawn available.";
        case GameValidatorStatus::kNoPlayerSpawn:
            return "No player spawns available.";
        case GameValidatorStatus::kNoCenterLevel:
            return "Game has no center level.";
        case GameValidatorStatus::kNoDefaultLevel:
            return "Game has no default level.";
        case GameValidatorStatus::kNoEndLevel:
            return "Game has no end level.";
        default:
            return "";
    }
}

std::string to_string(const GameValidatorResult& info) {
    switch (info.status) {
        case GameValidatorStatus::kEmptyLevel:
            return "Level \"" + info.name + "\" has no layers/tilesets.";
        case GameValidatorStatus::kNoDoorL:
            return "No left door available in level \"" + info.name + "\".";
        case GameValidatorStatus::kNoDoorR:
            return "No right door available in level \"" + info.name + "\".";
        case GameValidatorStatus::kNoFlagSpawn:
            return "No flag spawn available in level \"" + info.name + "\".";
        case GameValidatorStatus::kNoPlayerSpawn:
            return "No player spawns available in level \"" + info.name + "\".";
        default:
            return to_string(info.status);
    }
}

GameValidatorResult validateGameConfig(gsl::not_null<GameConfig*> gameConfig) {
    if (gameConfig->getLevels().empty()) {
        return {GameValidatorStatus::kEmptyGame};
    }

    bool hasCenterLevel = false;
    bool hasDefaultLevel = false;
    bool hasEndLevel = false;
    for (auto& item : gameConfig->getLevels()) {
        LevelConfig* level = item.second;
        if (level->getTilesets().empty()) {
            return {GameValidatorStatus::kEmptyLevel, level->getLevelFilename()};
        }

        if (level->getDoorL().x < 0 || level->getDoorL().y < 0) {
            return {GameValidatorStatus::kNoDoorL, level->getLevelFilename()};
        }
        if (level->getDoorR().x < 0 && level->getDoorR().y < 0) {
            return {GameValidatorStatus::kNoDoorR, level->getLevelFilename()};
        }
        if (level->getFlagSpawn().x < 0 && level->getFlagSpawn().y < 0) {
            return {GameValidatorStatus::kNoFlagSpawn, level->getLevelFilename()};
        }

        size_t playerSpawnCount = 0;
        for (auto& list : level->getPlayerSpawns()) {
            if (!list.empty()) {
                playerSpawnCount += list.size();
            }
        }

        if (playerSpawnCount < 2) {
            return {GameValidatorStatus::kNoPlayerSpawn, level->getLevelFilename()};
        }

        if (level->getLevelType() == LevelType::CENTER) {
            hasCenterLevel = true;
        }
        if (level->getLevelType() == LevelType::DEFAULT) {
            hasDefaultLevel = true;
        }
        if (level->getLevelType() == LevelType::END) {
            hasEndLevel = true;
        }
    }

    if (!hasCenterLevel) {
        return {GameValidatorStatus::kNoCenterLevel};
    }
    if (!hasDefaultLevel) {
        return {GameValidatorStatus::kNoDefaultLevel};
    }
    if (!hasEndLevel) {
        return {GameValidatorStatus::kNoEndLevel};
    }
    return {GameValidatorStatus::kOk};
}

}  // namespace parser
}  // namespace ctb
