// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <iostream>
#include <utility>

#include "engine/Window.hpp"
#include "engine/audio/SoundManager.hpp"
#include "engine/core/Game.hpp"
#include "engine/core/Level.hpp"
#include "engine/scene/Gun.hpp"
#include "engine/scene/Projectile.hpp"
#include "engine/util/Vector2d.hpp"

namespace ctb {
namespace engine {

Gun::Gun(SDL_Texture* texture,
         int animationWidth,
         int animationHeight,
         int animationCount,
         std::string projectileTexturePath,
         int projectileAnimationHeight,
         int projectileAnimationWidth,
         int projectileAnimationCount,
         int cooldown,
         float projectileSpeed,
         uint32_t projectileDamage)
    : Fist(texture, animationWidth, animationHeight, animationCount),
      m_projectileTexturePath(std::move(projectileTexturePath)),
      m_projectileFrameHeight(projectileAnimationHeight),
      m_projectileFrameWidth(projectileAnimationWidth),
      m_projectileNumFrames(projectileAnimationCount),
      m_angle(0),
      m_cooldown(cooldown),
      m_projectileSpeed(projectileSpeed),
      m_projectileDamage(projectileDamage) {}

bool Gun::isDropable() {
    return true;
}

bool Gun::isMelee() {
    return false;
}

void Gun::use() {
    m_use = true;
}

void Gun::unuse() {
    m_use = false;
}

void Gun::update() {
    if (inUse() && m_reloadCount < kMagazineCount) {
        int ticks = static_cast<int>(SDL_GetTicks());
        if (m_ammo == 0) {
            m_ammo = kMagazineCapacity;
            m_reloadStartTime = ticks;
            ++m_reloadCount;
        } else if (m_reloadStartTime + kReloadDelay - ticks < 0 &&
                   m_lastShot + m_cooldown - ticks < 0) {
            SoundManager::getInstance().playPew();
            Projectile* projectile = new Projectile(
                Window::getWindow().loadTexture(m_projectileTexturePath), m_projectileFrameWidth,
                m_projectileFrameHeight, m_projectileNumFrames, this, m_projectileDamage);

            Kinematics kinematics;
            kinematics.setDensity(5.0f);
            kinematics.setCategory(Level::PROJECTILE_CAT);
            kinematics.setMask(Level::GROUND_CAT | Level::PLAYER_CAT | Level::BOT_CAT);
            kinematics.setId(Game::PROJECTILE_ID);

            projectile->addToWorld(*m_body->GetWorld(), kinematics, m_body->GetPosition().x,
                                   m_body->GetPosition().y, m_angle, m_projectileSpeed);

            m_projectiles.push_back(projectile);
            m_lastShot = static_cast<int>(SDL_GetTicks());
            --m_ammo;
        }
    }

    for (Projectile* projectile : m_deleteProjectiles) {
        auto it = std::find(m_projectiles.begin(), m_projectiles.end(), projectile);
        if (it != m_projectiles.end()) {
            delete projectile;
            m_projectiles.erase(it);
        }
    }
    m_deleteProjectiles.clear();

    for (Projectile* projectile : m_projectiles) {
        projectile->update();
    }
    Fist::update();
}

void Gun::angleWeapon(double angle, Direction direction) {
    if (direction == Direction::Left) {
        m_angle = M_PI / 180 * (-angle + 180);
    } else {
        m_angle = M_PI / 180 * angle;
    }
}

void Gun::render() {
    for (Projectile* projectile : m_projectiles) {
        projectile->setOffset(m_offset);
        projectile->render();
    }

    nextAnimation();
    Vector2dT position = computeTargetPosition();

    // Just to be safe, copy target rect, don't modify make
    SDL_Rect target;
    target.x = position.x;
    target.y = position.y;
    target.w = m_targetRect.w;
    target.h = m_targetRect.h;

    auto* renderer = Window::getWindow().renderer();
    if (m_angle * 180 / M_PI > 90) {
        SDL_RenderCopyEx(renderer, m_texture, &m_sourceRect, &target, -m_angle * 180 / M_PI + 180,
                         nullptr, SDL_FLIP_HORIZONTAL);
    } else {
        SDL_RenderCopyEx(renderer, m_texture, &m_sourceRect, &target, -m_angle * 180 / M_PI,
                         nullptr, SDL_FLIP_NONE);
    }
}

void Gun::drop(bool xVelocity) {
    float xVel = xVelocity ? 4.0f : 0.0f;
    m_body->SetTransform(m_body->GetPosition() + b2Vec2(0, -2), 0);
    m_body->SetLinearVelocity(b2Vec2(xVel, -10));
}

void Gun::removeProjectile(Projectile* projectile) {
    if (std::find(m_deleteProjectiles.begin(), m_deleteProjectiles.end(), projectile) ==
        m_deleteProjectiles.end()) {
        m_deleteProjectiles.push_back(projectile);
    }
}

Gun::~Gun() {
    removeAllProjectiles();
}

void Gun::removeAllProjectiles() {
    for (auto& projectile : m_projectiles) {
        if (projectile) {
            delete projectile;
            projectile = nullptr;
        }
    }
    m_projectiles.clear();
}

}  // namespace engine
}  // namespace ctb
