// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_SCENE_BOT_HPP
#define ENGINE_SCENE_BOT_HPP

#include "engine/graphics/ActingRenderable.hpp"
#include "engine/util/Vector2d.hpp"

namespace ctb {
namespace parser {
class BotConfig;
class GameConfig;
}  // namespace parser

namespace engine {
class Player;
class Level;

/// Parent Class for all Bots
class Bot : public ActingRenderable {
   public:
    /**
     * @brief Create new Bot
     *
     * @param texture         the texture of the bot
     * @param animationWidth  the width of each animation
     * @param animationHeight the height of each animation
     * @param animationCount  the number of animation
     * @param level           the level the bot belongs to
     */
    Bot(SDL_Texture* texture,
        int animationWidth,
        int animationHeight,
        int animationCount,
        Level* level);

    /// Destructor
    ~Bot() override;

    /**
     * @brief static method for creating a bot and spawning it
     *
     * @parma world the world where the bot shall be spawned
     * @param type  the bot type to be spawned
     * @param pos   the position where it should be spawned
     * @param level the level the bot should be spawned in
     *
     * @return      A pointer to the created bot
     */
    static Bot* createBot(b2World& world, const std::string& type, Vector2dT pos, Level* level);

    /**
     * @brief static method for getting the corresponding BotConfig to name from gconf
     *
     * @param gconf The config to be searched in
     * @param name  The name to be searched for
     * @return      A pointer to the BotConfig or nullptr if none was found
     */
    static parser::BotConfig* getBotConfig(parser::GameConfig* gconf, const std::string& name);

    void prepareDelete();

    virtual void collideWithPlayer(Player* player) = 0;

    Level* getLevel();

   protected:
    Level* m_level;

    bool m_deleted;

    /// Handles the running logic
    virtual void run();

    /// Get the distance to the players
    float getDistance(Player* player);
};

}  // namespace engine

}  // namespace ctb

#endif
