// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_CORE_GAME_HPP
#define ENGINE_CORE_GAME_HPP

#include <map>
#include <utility>
#include <vector>

#include <parser/GameConfig.hpp>
#include <parser/LevelConfig.hpp>

#include "engine/core/Level.hpp"
#include "engine/graphics/SDLRenderable.hpp"
#include "engine/gui/Statusbar.hpp"
#include "engine/input/Input.hpp"
#include "engine/physics/LevelWorld.hpp"
#include "engine/scene/Flag.hpp"
#include "engine/scene/Player.hpp"

namespace ctb {
namespace engine {

class Level;

enum class Team { R2L, L2R };

enum class GameState { Stopped, Running, Paused };

static constexpr int RESPAWN_TIMEOUT = 1000;

/// Class representing a Game instance
class Game : public SDLRenderable {
   public:
    /**
     * @brief constructor
     *
     * @param mw     a pointer to the window
     * @param config the config this game is based on
     *
     * @throws runtime_error if not enough levels are present in the config
     */
    explicit Game(parser::GameConfig* config);

    /// Renders this game
    void render() override;

    /// Updates
    void update();

    /// Get the current game state
    inline GameState getGameState() { return m_state; }

    /// starts the game with the given players
    void startGame(std::vector<Player*> players);

    /**
     * @brief ends the game and displays the end menu
     *
     * @param teamWon the team that won
     */
    void endGame(Team teamWon);

    /// Pauses the game
    void pause();

    /// resumes the game
    void resume();

    /// get how many levels this game has
    int getLevelCount() const { return static_cast<int>(m_levelOrder.size()); }

    /// returns the current level index
    int getCurrentLevelIndex() const { return static_cast<int>(m_currentLevel); }

    /// return a pointer to the current level
    Level* getCurrentLevel() const { return m_levelOrder[m_currentLevel]; }

    /**
     * @brief switch to the next or previous level
     *
     * @param t which team reached the door?
     */
    void nextLevel(Team t);

    /**
     * @brief start the given level, on the side given by t (default is center)
     *
     * @param level the level index to start
     * @param t a pointer to the team which reached the door in the previous level
     *          start at the other door or in the center if null
     */
    void startLevel(uint32_t level, Team* t = nullptr);

    /// returns a reference to the players of Team R2L
    std::vector<Player*>& getR2LPlayers() { return m_players_R2L; }

    /// returns a reference to the players of Team L2R
    std::vector<Player*>& getL2RPlayers() { return m_players_L2R; }

    /*
     * @brief respawn the given player at the next respawn point, depending on the player's team
     *
     * @param player the player to respawn
     */
    void respawnPlayer(Player* player);

    /// calls setToRespawn(player, RESPAWN_TIMEOUT)
    void setToRespawn(Player* player);

    /**
     * @brief appends the player to the respawn queue to be respawned after timeout
     *
     * @param player the player to be respawned
     * @param timeout the timeout after which the player respawns
     * @param died if the player died and has to lose the flag etc.
     */
    void setToRespawn(Player* player, const int timeout, const bool died = true);

    /// Returns a pointer to the config of this game.
    parser::GameConfig* getConfig() const { return m_config; }

    /// Destructor
    ~Game() override;

    /// Defines how many Levels a game has
    static constexpr int LEVELCOUNT = 5;

    static_assert(LEVELCOUNT >= 5 && LEVELCOUNT % 2 != 0, "LEVELCOUNT has to be >=5 and odd!");

    /// Constants for identifying collision between different objects
    static constexpr int PLAYER_ID = 1;
    static constexpr int GROUND_ID = 2;
    static constexpr int FLAG_ID = 3;
    static constexpr int DOOR_ID = 4;
    static constexpr int BOT_ID = 5;
    static constexpr int PROJECTILE_ID = 6;
    static constexpr int WEAPON_ID = 7;

   private:
    /// Give all members of team hasFlag points for having the flag
    void flagScore(Team hasFlag);

    /**
     * @brief get next player spawn of given team inside camera bounds from config
     *
     * @param level     the current level to search in
     * @param team      the team to find a spawn for
     *
     * @return          the position of the best fitted spawn
     * @throws          runtime_error if no position was found fit
     */
    std::vector<int> getNextPlayerSpawn(Level* level, Team team);

    /// The underlying config for this game
    parser::GameConfig* m_config;

    /// An ordered vector of all the levels of this game
    std::vector<Level*> m_levelOrder;

    /// An ordered vector of all worlds of this game
    std::vector<LevelWorld*> m_levelWorlds;

    /// Players for the R2L team
    std::vector<Player*> m_players_R2L;

    /// Players for the L2R team
    std::vector<Player*> m_players_L2R;

    /// A Queue for the player respawns, mapping an player pointer to the time until respawn and a
    /// bool indicating if he died or not
    std::map<Player*, std::pair<int, bool>> m_respawnQueue;

    /// Save the last update time for the respawn queue
    Uint32 m_lastTicks;

    /// The current game state
    GameState m_state;

    /// Adds the player to the current level and calls setToRespawn
    void addPlayerToCurrentLevel(Player* player);

    /// The index of the current level in m_levelOrder
    uint32_t m_currentLevel;

    /// A pointer to this game's statusbar
    Statusbar* m_statusbar;
};

}  // namespace engine
}  // namespace ctb
#endif
