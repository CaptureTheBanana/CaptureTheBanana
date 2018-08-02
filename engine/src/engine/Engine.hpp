// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_ENGINE_HPP
#define ENGINE_ENGINE_HPP

#include <stack>
#include <string>
#include <vector>

#include <SDL.h>

#include "engine/Object.hpp"

namespace ctb {
namespace parser {
class GameConfig;
}

namespace engine {

class Game;
class Player;

class Engine : public Object {
   public:
    explicit Engine(const std::string& gamefile);

    virtual ~Engine();

    /// Resets the game and displays the main menu.
    void restart();

    void update();

    std::string getGameFile() const { return m_gamefile; }

    parser::GameConfig* getGameConfig();

    /// Returns the current game instance
    Game* getGame() const { return m_game; }

    /// Pause game
    void pauseGame(Player* player);

   private:
    /// the name of the game file this instance was started with
    std::string m_gamefile;

    /// A pointer to a game object
    Game* m_game;

    /// the game config
    ctb::parser::GameConfig* m_config;
};

}  // namespace engine
}  // namespace ctb

#endif  // ENGINE_ENGINE_HPP
