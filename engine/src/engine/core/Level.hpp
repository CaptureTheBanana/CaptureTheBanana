// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT License; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_CORE_LEVEL_HPP
#define ENGINE_CORE_LEVEL_HPP

#include <Box2D/Box2D.h>
#include <SDL.h>
#include <stack>
#include <string>
#include <vector>

#include <gsl/gsl>

#include "engine/core/Camera.hpp"
#include "engine/graphics/LayerRenderer.hpp"
#include "engine/graphics/TilesetRenderable.hpp"
#include "engine/physics/LevelContactListener.hpp"
#include "engine/physics/LevelWorld.hpp"
#include "engine/physics/PhysicalTileSet.hpp"
#include "engine/scene/Bot.hpp"
#include "engine/scene/Flag.hpp"

namespace ctb {
namespace parser {
class GameConfig;
class LevelConfig;
}  // namespace parser

namespace engine {

class Fist;
class LevelContactListener;
class Player;
class Door;
class Bot;

/**
 * @brief Represents a level in the game.
 */
class Level : public SDLRenderable {
   public:
    using WorldPtrT = gsl::not_null<LevelWorld*>;

    // constants for collision identification
    static constexpr uint16 PLAYER_CAT = 2;
    static constexpr uint16 GROUND_CAT = 4;
    static constexpr uint16 FLAG_CAT = 8;
    static constexpr uint16 DOOR_CAT = 16;
    static constexpr uint16 BOT_CAT = 32;
    static constexpr uint16 PROJECTILE_CAT = 64;
    static constexpr uint16 WEAPON_CAT = 128;

    static constexpr float32 PHYSICALTIMESTEPFREQUENCE = 40.0f;

    /**
     * @brief Construcotr
     *
     * @param gconf		    A pointer to a game config this level should use
     * @param lconf         A pointer to a level config this level should use. If flipped is true,
     *                      we take ownership!
     * @param flipped       wether the level should be flipped or not
     */
    Level(parser::GameConfig* gconf,
          parser::LevelConfig* lconf,
          WorldPtrT world,
          bool flipped = false);

    /// Renders the level
    void render() override;

    /// Destructor
    ~Level() override;

    /// Updates the elements within the level according
    /// to the given SDL keyboard states
    void update();

    /// returns a reference to this level's camera
    Camera& getCamera() { return m_camera; }

    /// returns if the level is flipped or not
    bool isFlipped() const { return m_flipped; }

    /// Adds a renderable on the given layer to the level
    void addRenderable(TextureBasedRenderable* renderable, int layer = 0);

    /// Adds an player to the level on the given layer
    void addPlayer(Player* player, int layer);

    /// add a bot to the level
    void addBot();

    /// delete the given bot
    void deleteBot(Bot* bot);

    /**
     * @brief Add the given tileset with the given config to this level
     *
     * @param tiles the tileset to add
     * @param config the config
     */
    void addLevelTiles(PhysicalTileSet* tiles, parser::TilesetConfig& config);

    /// get this level's config
    parser::LevelConfig* getConfig() const { return m_lconf; }

    /// get this level's doors
    std::vector<Door*>& getDoors() { return m_doors; }

    /// get this level's flag
    Flag* getFlag() { return m_flag; }

    /// adds a dropped weapon to the level
    void addWeapon();

    /// drops weapon and adds it to world
    void spawnWeapon(Fist* weapon);

    /// drops weapon on level
    void dropWeapon(Fist* weapon, bool xVelocity = true);

    /// removes weapon from level (e.g if player collects it)
    void removeWeapon(Fist* weapon);

    /// respawns this levels flag and destroys its joint if needed
    void respawnFlag();

   private:
    /// Is this level flipped?
    bool m_flipped{false};

    /// Physical Environment
    WorldPtrT m_world;

    /// A camera
    Camera m_camera;

    /// A layer renderer for correct rendering of the renderables
    LayerRenderer m_layers;

    /// all bots in this level
    std::vector<Bot*> m_bots;

    /// garbage collector for the bots
    std::stack<Bot*> m_GCBots;

    /// all physical objects in this level
    std::vector<PhysicalRenderable*> m_objects;

    /// all doors in this level
    std::vector<Door*> m_doors;

    /// this level's config
    parser::LevelConfig* m_lconf;

    /// the gameconfig of the game this level is in
    parser::GameConfig* m_gconf;

    /// A tile array
    TilesetRenderable* m_tiles;

    /// this level's flag
    Flag* m_flag;

    /// this weapons of this level
    std::vector<Fist*> m_weapons;
};

}  // namespace engine
}  // namespace ctb

#endif  // ENGINE_CORE_LEVEL_HPP
