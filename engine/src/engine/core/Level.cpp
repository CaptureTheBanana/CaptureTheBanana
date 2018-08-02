// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT License; see LICENSE file in the root of this
// project for details.

#include <iostream>

#include <SDL_image.h>
#include <parser/BotConfig.hpp>
#include <parser/GameConfig.hpp>
#include <parser/GameParser.hpp>
#include <parser/LevelConfig.hpp>
#include <parser/LevelParser.hpp>

#include "engine/Window.hpp"
#include "engine/core/Game.hpp"
#include "engine/core/Level.hpp"
#include "engine/graphics/Background.hpp"
#include "engine/physics/ActingKinematics.hpp"
#include "engine/scene/Bot.hpp"
#include "engine/scene/Door.hpp"
#include "engine/scene/Gun.hpp"
#include "engine/scene/Player.hpp"
#include "engine/scene/Ufo.hpp"
#include "engine/scene/Zombie.hpp"
#include "engine/util/Random.hpp"
#include "engine/util/Vector2d.hpp"

namespace ctb {
namespace engine {

namespace {
using parser::GameConfig;
using parser::LevelConfig;
}  // namespace

Level::Level(GameConfig* gconf, LevelConfig* lconf, WorldPtrT world, bool flipped)
    : m_flipped(flipped),
      m_world(world),
      m_camera(0,
               0,
               Window::getWindow().w(),
               Window::getWindow().h(),
               lconf->getPixelWidth(),
               lconf->getPixelHeight()),
      m_layers(&m_camera),
      m_lconf(lconf),
      m_gconf(gconf) {
    for (auto layer : lconf->getTilesets()) {
        addLevelTiles(new PhysicalTileSet(layer, lconf, flipped), layer);
    }
    for (auto background : lconf->getBackgrounds()) {
        addRenderable(new Background(background, lconf->getWidth() * lconf->getTileWidth(),
                                     lconf->getHeight() * lconf->getTileHeight()),
                      background.getLayer());
    }

    m_world->getWorld()->SetGravity(b2Vec2(0, lconf->getGravitation()));

    // maybe flip level
    if (flipped) {
        lconf->flipPoints();
    }

    // create flag
    SDL_Texture* tex = Window::getWindow().loadTexture(gconf->getFlagFilename());
    int w, h;
    SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
    m_flag = new Flag(tex, w, h, 1);
    m_layers.addRenderable(m_flag, gconf->getPlayerLayer(), true);
    m_camera.setFocus(m_flag);
    m_objects.push_back(m_flag);

    // TODO(felix): Use pointer
    m_flag->addToThisWorld(*m_world->getWorld());
    respawnFlag();

    // create doors
    tex = Window::getWindow().loadTexture(gconf->getDoorFilename());
    SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);

    Door* door = new Door(Team::R2L, tex, w, h, 1, gconf->getDoorOffset());
    Vector2dT pos = lconf->getDoorL();
    pos = lconf->toWorld(pos);
    door->setPosition(pos);
    m_layers.addRenderable(door, gconf->getPlayerLayer() - 1, true);
    m_objects.push_back(door);

    // TODO(felix): Use pointer
    door->addToThisWorld(*m_world->getWorld());
    m_doors.push_back(door);

    door = new Door(Team::L2R, tex, w, h, 1, gconf->getDoorOffset());
    pos = lconf->getDoorR();
    pos = lconf->toWorld(pos);
    door->setPosition(pos);
    m_layers.addRenderable(door, gconf->getPlayerLayer() - 1, true);
    m_objects.push_back(door);

    // TODO(felix): Use pointer
    door->addToThisWorld(*m_world->getWorld());
    m_doors.push_back(door);
}

void Level::addRenderable(TextureBasedRenderable* renderable, int layer) {
    Expects(renderable != nullptr);
    m_layers.addRenderable(renderable, layer, true);
}

void Level::addPlayer(Player* player, int layer) {
    if (std::find(m_objects.begin(), m_objects.end(), player) == m_objects.end()) {
        m_layers.addRenderable(player, layer, false);
        m_objects.push_back(player);
    }
    ActingKinematics k;
    k.setRestitution(0.05f);
    k.setId(Game::PLAYER_ID);
    k.setCategory(PLAYER_CAT);
    k.setMask(GROUND_CAT | FLAG_CAT | DOOR_CAT | BOT_CAT | WEAPON_CAT | PROJECTILE_CAT);

    // TODO(felix): Use pointer
    player->addToWorld(*m_world->getWorld(), k);
}

void Level::addBot() {
    std::vector<parser::TypeSpawn>& botSpawn = m_lconf->getBotSpawns();
    if (m_bots.size() < 5 && !botSpawn.empty()) {
        int pickPos = Random::getInt(0, static_cast<int>(botSpawn.size()) - 1);
        Vector2dT pos = m_lconf->toWorld(botSpawn.at(static_cast<size_t>(pickPos)).getPosition());

        // TODO(felix): Use pointer
        Bot* bot = Bot::createBot(*m_world->getWorld(),
                                  botSpawn.at(static_cast<size_t>(pickPos)).getType(), pos, this);
        if (bot != nullptr) {
            m_bots.push_back(bot);
        }
    }
}

void Level::deleteBot(Bot* bot) {
    auto it = std::find(m_bots.begin(), m_bots.end(), bot);
    if (it != m_bots.end()) {
        m_bots.erase(it);
    }
}

void Level::addLevelTiles(PhysicalTileSet* tiles, parser::TilesetConfig& config) {
    if (tiles) {
        m_tiles = tiles;
        m_layers.addRenderable(tiles, config.getLayer(), true);

        Kinematics k;
        k.setFriction(config.getFriction());
        k.setId(Game::GROUND_ID);
        k.setCategory(GROUND_CAT);

        uint16 mask = DOOR_CAT;
        if (config.canCollidePlayers()) {
            mask |= PLAYER_CAT | FLAG_CAT | WEAPON_CAT;
        }
        if (config.canCollideBots()) {
            mask |= BOT_CAT;
        }
        if (config.canCollideBots()) {
            mask |= PROJECTILE_CAT;
        }
        if (!config.canCollideBots() && !config.canCollidePlayers() &&
            !config.canCollideProjectiles()) {
            mask = 0;
        }
        k.setMask(mask);

        // TODO(felix): Use pointer
        tiles->addToWorld(*m_world->getWorld(), k);
    }
}

void Level::addWeapon() {
    if (!m_lconf->getWeaponSpawns().empty()) {
        parser::TypeSpawn spawn = m_lconf->getWeaponSpawns().at(static_cast<size_t>(
            Random::getInt(0, static_cast<int>(m_lconf->getWeaponSpawns().size() - 1))));

        if (spawn.getType() == "gun") {
            // search for weapons and projectiles
            auto weaponConfig = m_gconf->getWeapons().at(0);
            for (auto& config : m_gconf->getWeapons()) {
                if (config->getName() == "gun") {
                    weaponConfig = config;
                }
            }
            auto projectileConfig = m_gconf->getProjectiles().at(0);
            for (auto& config : m_gconf->getProjectiles()) {
                if (config->getName() == "bullet") {
                    projectileConfig = config;
                }
            }

            // get texture information
            SDL_Texture* gunTexture = Window::getWindow().loadTexture(weaponConfig->getFilename());
            SDL_Texture* projectileTexture =
                Window::getWindow().loadTexture(projectileConfig->getFilename());

            int gunWidth, gunHeight;
            SDL_QueryTexture(gunTexture, nullptr, nullptr, &gunWidth, &gunHeight);
            int projectileWidth, projectileHeight;
            SDL_QueryTexture(projectileTexture, nullptr, nullptr, &projectileWidth,
                             &projectileHeight);
            SDL_DestroyTexture(projectileTexture);
            projectileTexture = nullptr;

            /// spawn gun
            Gun* gun = new Gun(gunTexture, gunWidth, gunHeight, 1, projectileConfig->getFilename(),
                               projectileHeight, projectileWidth, 1,
                               static_cast<int>(weaponConfig->getAttackspeed()),
                               static_cast<float>(weaponConfig->getRange()),
                               static_cast<uint32_t>(weaponConfig->getDamage()));
            gun->setPosition({spawn.getPosition().x * m_lconf->getTileWidth(),
                              spawn.getPosition().y * m_lconf->getTileHeight()});
            spawnWeapon(gun);
        }
    }
}

void Level::spawnWeapon(Fist* weapon) {
    if (weapon) {
        Kinematics k(0.5f, 0.5f, 0.2f);
        k.setDensity(0.0f);
        k.setId(Game::WEAPON_ID);
        k.setCategory(Level::WEAPON_CAT);
        k.setMask(Level::PLAYER_CAT | Level::GROUND_CAT);

        weapon->addToWorld(*m_world->getWorld(), k);

        dropWeapon(weapon, false);
    }
}

void Level::dropWeapon(Fist* weapon, bool xVelocity) {
    if (weapon) {
        m_weapons.push_back(weapon);
        weapon->drop(xVelocity);
    }
}

void Level::removeWeapon(Fist* weapon) {
    if (weapon) {
        std::vector<Fist*>::iterator it = std::find(m_weapons.begin(), m_weapons.end(), weapon);

        if (it != m_weapons.end()) {
            m_weapons.erase(it);
        }
    }
}

void Level::respawnFlag() {
    m_flag->setInUse(false);
    m_flag->reset();
    m_flag->destroyJoint();
    Vector2dT pos = m_lconf->getFlagSpawn();
    pos = m_lconf->toWorld(pos);
    m_flag->setPosition(pos);
}

void Level::update() {
    // Run physics
    for (auto obj : m_objects) {
        obj->update();
    }

    if (Random::getInt(0, 10000) < 25) {
        addBot();
    }

    // Update bots
    for (auto obj : m_bots) {
        obj->update();
    }

    if (m_camera.checkBounds(m_flag)) {
        respawnFlag();
    }

    float32 timeStep = 1.0f / PHYSICALTIMESTEPFREQUENCE;
    int32 velocityIterations = 5;
    int32 positionIterations = 6;

    m_world->getWorld()->Step(timeStep, velocityIterations, positionIterations);
    m_world->getListener()->update();

    for (Fist* fist : m_weapons) {
        fist->update();
    }

    // 0.04% spawnchance for weapons per loop (á 60FPS)
    if (Random::getInt(0, 10000) < 4) {
        addWeapon();
    }
}

void Level::render() {
    m_layers.render();

    for (Fist* fist : m_weapons) {
        fist->setOffset(m_camera.getPosition());
        fist->render();
    }

    for (auto& item : m_bots) {
        item->setOffset(m_camera.getPosition());
        item->render();
    }
}

Level::~Level() {
    Expects(!m_world->getWorld()->IsLocked());

    if (m_flipped) {
        delete m_lconf;
    }

    for (auto& item : m_bots) {
        delete item;
        item = nullptr;
    }

    for (auto& m_weapon : m_weapons) {
        delete m_weapon;
        m_weapon = nullptr;
    }
}

}  // namespace engine
}  // namespace ctb
