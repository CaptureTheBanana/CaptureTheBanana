// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef PARSER_GAMECONFIG_HPP
#define PARSER_GAMECONFIG_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

#include "parser/BotConfig.hpp"
#include "parser/LevelConfig.hpp"
#include "parser/PlayerConfig.hpp"
#include "parser/ProjectileConfig.hpp"
#include "parser/WeaponConfig.hpp"

namespace ctb {
namespace parser {

/// Stores configuration of the game with all possible levels, sprites, ...
class GameConfig {
   public:
    GameConfig() = default;

    ~GameConfig() {
        for (auto p : m_levels) {
            delete p.second;
        }
        for (PlayerConfig* pc : m_players) {
            delete pc;
        }
        for (BotConfig* bc : m_bots) {
            delete bc;
        }
        for (WeaponConfig* wc : m_weapons) {
            delete wc;
        }
        for (ProjectileConfig* pc : m_projectiles) {
            delete pc;
        }
    }

    // Disable copy constructor and copy-assignment.
    GameConfig(const GameConfig&) = delete;
    GameConfig& operator=(const GameConfig&) = delete;

    // setter & getter methods
    std::string getFontsFolder() { return m_fontsFolder; }
    void setFontsFolder(std::string folder) { m_fontsFolder = folder; }
    std::string getSoundsFolder() { return m_soundsFolder; }
    void setSoundsFolder(std::string folder) { m_soundsFolder = folder; }
    std::string getInputsFile() { return m_inputsFile; }
    void setInputsFile(std::string folder) { m_inputsFile = folder; }
    int getPlayerLayer() { return m_playerLayer; }
    void setPlayerLayer(int layer) { m_playerLayer = layer; }
    int getDoorOffset() { return m_doorOffset; }
    void setDoorOffset(int offset) { m_doorOffset = offset; }
    int getPlayerOffset() { return m_playerOffset; }
    void setPlayerOffset(int offset) { m_playerOffset = offset; }
    std::string getFlagFilename() { return m_flagFilename; }
    void setFlagFilename(std::string file) { m_flagFilename = file; }
    std::string getDoorFilename() { return m_doorFilename; }
    void setDoorFilename(std::string file) { m_doorFilename = file; }
    std::multimap<LevelType, LevelConfig*>& getLevels() { return m_levels; }
    std::vector<PlayerConfig*>& getPlayers() { return m_players; }
    std::vector<BotConfig*>& getBots() { return m_bots; }
    std::vector<WeaponConfig*>& getWeapons() { return m_weapons; }
    std::vector<ProjectileConfig*>& getProjectiles() { return m_projectiles; }

    /// \brief Adds the given level to the game.
    ///
    /// \note Take ownership
    ///
    /// \param level Level
    void addLevel(LevelConfig* level) {
        m_levels.insert(std::make_pair(level->getLevelType(), level));
    }

    /// \brief Removes the given level from the game and and frees memory.
    ///
    /// \param level Level
    /// \return bool Returns true on success.
    bool removeLevel(LevelConfig* level) {
        auto it = m_levels.find(level->getLevelType());
        for (/*it*/; it != m_levels.end(); ++it) {
            // Search for same memory address
            if (it->second == level) {
                break;
            }
        }

        if (it == m_levels.end()) {
            return false;
        }

        // Remove level and free memory
        m_levels.erase(it);
        delete level;
        return true;
    }

   private:
    int m_playerLayer;
    int m_doorOffset;
    int m_playerOffset;
    std::string m_fontsFolder;
    std::string m_soundsFolder;
    std::string m_inputsFile;
    std::string m_flagFilename;
    std::string m_doorFilename;
    std::multimap<LevelType, LevelConfig*> m_levels;
    std::vector<PlayerConfig*> m_players;
    std::vector<BotConfig*> m_bots;
    std::vector<WeaponConfig*> m_weapons;
    std::vector<ProjectileConfig*> m_projectiles;
};

}  // namespace parser
}  // namespace ctb

#endif
