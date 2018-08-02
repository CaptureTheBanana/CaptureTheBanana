// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <cmath>
#include <limits>

#include <boost/foreach.hpp>
#include <boost/range/join.hpp>
#include <parser/BotConfig.hpp>
#include <parser/GameConfig.hpp>

#include "engine/Engine.hpp"
#include "engine/Window.hpp"
#include "engine/core/GC.hpp"
#include "engine/core/Game.hpp"
#include "engine/core/Level.hpp"
#include "engine/scene/Bot.hpp"
#include "engine/scene/Player.hpp"
#include "engine/scene/Ufo.hpp"
#include "engine/scene/Zombie.hpp"

namespace ctb {
namespace engine {
namespace {
using ctb::parser::BotConfig;
using ctb::parser::GameConfig;
}  // namespace

Bot::Bot(SDL_Texture* texture,
         int animationWidth,
         int animationHeight,
         int animationCount,
         Level* level)
    : ActingRenderable(texture, animationWidth, animationHeight, animationCount), m_deleted(false) {
    m_level = level;
}

Bot* Bot::createBot(b2World& world, const std::string& type, Vector2dT pos, Level* level) {
    GameConfig* gconf = Window::getEngine().getGame()->getConfig();

    Bot* bot = nullptr;
    BotConfig* conf = getBotConfig(gconf, type);
    if (!conf) {
        return nullptr;
    }
    std::string filename = conf->getFilename();
    if (type == "zombie") {
        bot = new Zombie(Window::getWindow().loadTexture(filename), 32, 64, 1, level);
    } else if (type == "ufo") {
        bot = new Ufo(Window::getWindow().loadTexture(getBotConfig(gconf, "ufo")->getFilename()),
                      56, 48, 5, level);
    } else {
        return nullptr;
    }
    if (!bot) {
        return nullptr;
    }

    ActingKinematics k;
    k.setId(Game::BOT_ID);
    k.setCategory(Level::BOT_CAT);
    k.setMask(Level::GROUND_CAT | Level::PLAYER_CAT | Level::PROJECTILE_CAT);

    bot->setPosition(pos);
    bot->addToWorld(world, k);
    return bot;
}

BotConfig* Bot::getBotConfig(GameConfig* gconf, const std::string& name) {
    auto& vec = gconf->getBots();
    auto it =
        std::find_if(vec.begin(), vec.end(), [name](BotConfig* c) { return c->getName() == name; });
    if (it == vec.end()) {
        return nullptr;
    }
    return *it;
}

void Bot::run() {
    Game* game = Window::getEngine().getGame();
    std::vector<Player*> t1 = game->getR2LPlayers();
    std::vector<Player*> t2 = game->getL2RPlayers();

    Player* player = nullptr;
    float distance = std::numeric_limits<float>::infinity();
    for (Player* a : boost::join(t1, t2)) {
        float current_distance = getDistance(a);
        if (current_distance < distance) {
            distance = current_distance;
            player = a;
        }
    }

    if (player) {
        if (player->worldPosition().x < this->worldPosition().x) {
            moveLeft();
        }
        if (player->worldPosition().x > this->worldPosition().x) {
            moveRight();
        }
    }
}

float Bot::getDistance(Player* player) {
    if (player) {
        return std::sqrt(
            std::pow(
                static_cast<float>(std::fabs(player->worldPosition().x - this->worldPosition().x)),
                2.0f) +
            std::pow(
                static_cast<float>(std::fabs(player->worldPosition().y - this->worldPosition().y)),
                2.0f));
    }
    return std::numeric_limits<float>::infinity();
}

void Bot::prepareDelete() {
    if (!m_deleted) {
        m_level->deleteBot(this);
        GC::add(this);
        m_deleted = true;
    }
}

Level* Bot::getLevel() {
    return m_level;
}

Bot::~Bot() {
    if (m_body) {
        m_body->GetWorld()->DestroyBody(m_body);
        m_body = nullptr;
    }
}

}  // namespace engine
}  // namespace ctb
