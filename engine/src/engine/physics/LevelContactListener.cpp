// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <gsl/gsl>

#include "engine/audio/SoundManager.hpp"
#include "engine/core/Game.hpp"
#include "engine/graphics/PhysicalRenderable.hpp"
#include "engine/physics/LevelContactListener.hpp"
#include "engine/physics/PhysicalObject.hpp"
#include "engine/scene/Bot.hpp"
#include "engine/scene/Door.hpp"
#include "engine/scene/Fist.hpp"
#include "engine/scene/Flag.hpp"
#include "engine/scene/Gun.hpp"
#include "engine/scene/Player.hpp"
#include "engine/scene/Projectile.hpp"
#include "engine/util/Random.hpp"

namespace ctb {
namespace engine {

LevelContactListener::LevelContactListener() : m_a(nullptr), m_b(nullptr), m_player(nullptr) {}

void LevelContactListener::update() {
    if (m_a && m_b) {
        m_a->createJoint(m_b);
        m_a = m_b = nullptr;
    }
    if (m_player) {
        m_player->doorReached();
        m_player = nullptr;
    }
}

void LevelContactListener::BeginContact(b2Contact* contact) {
    void* a = contact->GetFixtureA()->GetBody()->GetUserData();
    void* b = contact->GetFixtureB()->GetBody()->GetUserData();
    if (a == nullptr || b == nullptr) {
        return;
    }
    PhysicalObject* a_obj = static_cast<PhysicalObject*>(a);
    PhysicalObject* b_obj = static_cast<PhysicalObject*>(b);

    // If Bots or Players are falling on the ground

    if (a_obj->getCollisionId() == Game::GROUND_ID && b_obj->getCollisionId() == Game::PLAYER_ID) {
        static_cast<ActingRenderable*>(b)->setOnGround(true);
    }

    if (b_obj->getCollisionId() == Game::GROUND_ID && a_obj->getCollisionId() == Game::PLAYER_ID) {
        static_cast<ActingRenderable*>(a)->setOnGround(true);
    }

    if (a_obj->getCollisionId() == Game::GROUND_ID && b_obj->getCollisionId() == Game::BOT_ID) {
        static_cast<ActingRenderable*>(b)->setOnGround(true);
    }

    if (b_obj->getCollisionId() == Game::GROUND_ID && a_obj->getCollisionId() == Game::BOT_ID) {
        static_cast<ActingRenderable*>(a)->setOnGround(true);
    }

    if (a_obj->getCollisionId() == Game::FLAG_ID && b_obj->getCollisionId() == Game::PLAYER_ID) {
        collectFlag(dynamic_cast<Player*>(b_obj), dynamic_cast<Flag*>(a_obj));
    }

    if (b_obj->getCollisionId() == Game::FLAG_ID && a_obj->getCollisionId() == Game::PLAYER_ID) {
        collectFlag(dynamic_cast<Player*>(a_obj), dynamic_cast<Flag*>(b_obj));
    }

    if (a_obj->getCollisionId() == Game::DOOR_ID && b_obj->getCollisionId() == Game::PLAYER_ID) {
        doorReached(contact, dynamic_cast<Player*>(b_obj), dynamic_cast<Door*>(a_obj));
    }

    if (b_obj->getCollisionId() == Game::DOOR_ID && a_obj->getCollisionId() == Game::PLAYER_ID) {
        doorReached(contact, dynamic_cast<Player*>(a_obj), dynamic_cast<Door*>(b_obj));
    }

    if (a_obj->getCollisionId() == Game::PLAYER_ID && b_obj->getCollisionId() == Game::BOT_ID) {
        if (a_obj != nullptr && b_obj != nullptr) {
            collidedBotPlayer(dynamic_cast<Player*>(a_obj), dynamic_cast<Bot*>(b_obj));
        }
    }

    if (b_obj->getCollisionId() == Game::PLAYER_ID && a_obj->getCollisionId() == Game::BOT_ID) {
        if (b_obj != nullptr && a_obj != nullptr) {
            collidedBotPlayer(dynamic_cast<Player*>(b_obj), dynamic_cast<Bot*>(a_obj));
        }
    }
}

void LevelContactListener::doorReached(b2Contact* contact, Player* player, Door* door) {
    Expects(player != nullptr && door != nullptr);
    if (player->hasFlag() && player->getTeam() == door->getTeam()) {
        m_player = player;
    } else {
        contact->SetEnabled(false);
    }
}

void LevelContactListener::flagOwned(Player* player, Flag* flag) {
    Expects(player != nullptr && flag != nullptr);
    flag->setInUse(true);
    player->setHasFlag(true);
}

bool LevelContactListener::isFlagInUse(PhysicalObject* obj) {
    Flag* flag = dynamic_cast<Flag*>(obj);
    Expects(flag != nullptr);
    return flag->isInUse();
}

void LevelContactListener::collidedBotPlayer(Player* player, Bot* bot) {
    bot->collideWithPlayer(player);
}

void LevelContactListener::EndContact(b2Contact* contact) {
    void* a = contact->GetFixtureA()->GetBody()->GetUserData();
    void* b = contact->GetFixtureB()->GetBody()->GetUserData();
    if (a == nullptr || b == nullptr) {
        return;
    }
    PhysicalObject* a_obj = static_cast<PhysicalObject*>(a);
    PhysicalObject* b_obj = static_cast<PhysicalObject*>(b);

    // If Bots or Player are leaving the ground

    if (a_obj->getCollisionId() == Game::GROUND_ID && b_obj->getCollisionId() == Game::PLAYER_ID) {
        static_cast<ActingRenderable*>(contact->GetFixtureB()->GetBody()->GetUserData())
            ->setOnGround(false);
    }

    if (b_obj->getCollisionId() == Game::GROUND_ID && a_obj->getCollisionId() == Game::PLAYER_ID) {
        static_cast<ActingRenderable*>(contact->GetFixtureA()->GetBody()->GetUserData())
            ->setOnGround(false);
    }

    if (a_obj->getCollisionId() == Game::GROUND_ID && b_obj->getCollisionId() == Game::BOT_ID) {
        static_cast<ActingRenderable*>(contact->GetFixtureB()->GetBody()->GetUserData())
            ->setOnGround(false);
    }

    if (b_obj->getCollisionId() == Game::GROUND_ID && a_obj->getCollisionId() == Game::BOT_ID) {
        static_cast<ActingRenderable*>(contact->GetFixtureA()->GetBody()->GetUserData())
            ->setOnGround(false);
    }
}

void LevelContactListener::PreSolve(b2Contact* contact, const b2Manifold* /*oldManifold*/) {
    void* a = contact->GetFixtureA()->GetBody()->GetUserData();
    void* b = contact->GetFixtureB()->GetBody()->GetUserData();
    PhysicalObject* a_obj = static_cast<PhysicalObject*>(a);
    PhysicalObject* b_obj = static_cast<PhysicalObject*>(b);

    // flag
    if (a_obj != nullptr && a_obj->getCollisionId() == Game::FLAG_ID &&
        dynamic_cast<Flag*>(a_obj)->isInUse()) {
        contact->SetEnabled(false);
    }

    if (b_obj != nullptr && b_obj->getCollisionId() == Game::FLAG_ID &&
        dynamic_cast<Flag*>(b_obj)->isInUse()) {
        contact->SetEnabled(false);
    }

    if (a != nullptr && b != nullptr) {
        if (a_obj->getCollisionId() == Game::PROJECTILE_ID &&
            b_obj->getCollisionId() == Game::WEAPON_ID) {
            contact->SetEnabled(false);
        }
        if (b_obj->getCollisionId() == Game::PROJECTILE_ID &&
            a_obj->getCollisionId() == Game::WEAPON_ID) {
            contact->SetEnabled(false);
        }
    }

    if (a == nullptr || b == nullptr) {
        if (a != nullptr && a_obj->getCollisionId() == Game::PROJECTILE_ID) {
            dynamic_cast<Projectile*>(a_obj)->getGun()->removeProjectile(
                dynamic_cast<Projectile*>(a_obj));
        } else if (b != nullptr && b_obj->getCollisionId() == Game::PROJECTILE_ID) {
            dynamic_cast<Projectile*>(b_obj)->getGun()->removeProjectile(
                dynamic_cast<Projectile*>(b_obj));
        }

        if (a_obj != nullptr && a_obj->getCollisionId() == Game::WEAPON_ID) {
            if (dynamic_cast<Fist*>(a_obj)->getUser() != nullptr) {
                contact->SetEnabled(false);
            }
        }
        if (b_obj != nullptr && b_obj->getCollisionId() == Game::WEAPON_ID) {
            if (dynamic_cast<Fist*>(b_obj)->getUser() != nullptr) {
                contact->SetEnabled(false);
            }
        }

        return;
    }

    // weapons
    if (a_obj->getCollisionId() == Game::WEAPON_ID && b_obj->getCollisionId() == Game::PLAYER_ID) {
        collisionPlayerWeapon(dynamic_cast<Player*>(b_obj), dynamic_cast<Fist*>(a_obj), contact);
    }
    if (b_obj->getCollisionId() == Game::WEAPON_ID && a_obj->getCollisionId() == Game::PLAYER_ID) {
        collisionPlayerWeapon(dynamic_cast<Player*>(a_obj), dynamic_cast<Fist*>(b_obj), contact);
    }
    if (a_obj->getCollisionId() == Game::WEAPON_ID && b_obj->getCollisionId() == Game::GROUND_ID) {
        if (dynamic_cast<Fist*>(a_obj)->getUser() != nullptr) {
            contact->SetEnabled(false);
        }
    }
    if (b_obj->getCollisionId() == Game::WEAPON_ID && a_obj->getCollisionId() == Game::GROUND_ID) {
        if (dynamic_cast<Fist*>(b_obj)->getUser() != nullptr) {
            contact->SetEnabled(false);
        }
    }

    // projectiles
    if (a_obj->getCollisionId() == Game::PROJECTILE_ID) {
        collisionWithProjectile(dynamic_cast<Projectile*>(a_obj), b_obj, contact);
    }

    if (b_obj->getCollisionId() == Game::PROJECTILE_ID) {
        collisionWithProjectile(dynamic_cast<Projectile*>(b_obj), a_obj, contact);
    }

    // If the player has no flag collision with doors and players is disabled.
    if (a_obj->getCollisionId() == Game::DOOR_ID && b_obj->getCollisionId() == Game::PLAYER_ID) {
        doorIgnoring(dynamic_cast<Door*>(a_obj), dynamic_cast<Player*>(b_obj), contact);
    }

    if (b_obj->getCollisionId() == Game::DOOR_ID && a_obj->getCollisionId() == Game::PLAYER_ID) {
        doorIgnoring(dynamic_cast<Door*>(b_obj), dynamic_cast<Player*>(a_obj), contact);
    }
}

void LevelContactListener::meleeWithCooldown(Player* attacking, Player* hurt) {
    if (attacking == nullptr || hurt == nullptr) {
        return;
    }

    Uint32 ticks = SDL_GetTicks();
    if (ticks > attacking->nextMeleeTick()) {
        SoundManager::getInstance().playHit();
        attacking->resetMeleeTick(ticks);

        uint32_t damage = 15u + static_cast<uint32_t>(Random::getInt(0, 10));
        hurt->addDamage(damage, attacking);
    }
}

void LevelContactListener::collectFlag(Player* player, Flag* flag) {
    Expects(player != nullptr && flag != nullptr);

    if (isFlagInUse(flag)) {
    } else {
        m_a = flag;
        m_b = player;
        flagOwned(player, flag);
    }
}

void LevelContactListener::collisionPlayerWeapon(Player* player, Fist* weapon, b2Contact* contact) {
    Expects(player != nullptr && weapon != nullptr && contact != nullptr);

    contact->SetEnabled(false);

    if (weapon->getUser() == nullptr) {
        // collect weapon
        player->collectWeapon(weapon);
    } else if (weapon->isMelee() && weapon->getUser()->getTeam() != player->getTeam()) {
        // melee damage

        meleeWithCooldown(weapon->getUser(), player);
    }
}

void LevelContactListener::collisionWithProjectile(Projectile* projectile,
                                                   PhysicalObject* obj,
                                                   b2Contact* contact) {
    Expects(projectile != nullptr && obj != nullptr && contact != nullptr);

    // If a projectile collides with an player (no friendly fire)
    if (obj->getCollisionId() == Game::PLAYER_ID) {
        Player* player = dynamic_cast<Player*>(obj);

        if (projectile->getUser() == nullptr) {
            projectile->getGun()->removeProjectile(projectile);
            contact->SetEnabled(false);
        } else if (projectile->getUser()->getTeam() != player->getTeam()) {
            SoundManager::getInstance().playDamage();
            player->addDamage(projectile->getDamage());
            projectile->getUser()->alterScore(PlayerScoreFrom::SCORE_PROJECTILE);
            projectile->getGun()->removeProjectile(projectile);
        } else {
            contact->SetEnabled(false);
        }
    }
    // If a projectile collides with the ground
    if (obj->getCollisionId() == Game::GROUND_ID) {
        projectile->getGun()->removeProjectile(projectile);
    }
    // if a projectile collides with a bot
    if (obj->getCollisionId() == Game::BOT_ID) {
        if (projectile->getUser() != nullptr) {
            projectile->getUser()->alterScore(PlayerScoreFrom::SCORE_BOT);
        }
        projectile->getGun()->removeProjectile(projectile);
        collidedBotPlayer(nullptr, dynamic_cast<Bot*>(obj));
    }
}

void LevelContactListener::doorIgnoring(Door* door, Player* player, b2Contact* contact) {
    Expects(door != nullptr && player != nullptr && contact != nullptr);

    if (!player->hasFlag() || player->getTeam() != door->getTeam()) {
        contact->SetEnabled(false);
    }
}

}  // namespace engine
}  // namespace ctb
