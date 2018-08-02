// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/scene/Zombie.hpp"
#include "engine/Window.hpp"
#include "engine/core/Game.hpp"
#include "engine/util/Random.hpp"

namespace ctb {
namespace engine {

Zombie::Zombie(SDL_Texture* texture,
               int animationWidth,
               int animationHeight,
               int animationCount,
               Level* level)
    : Bot(texture, animationWidth, animationHeight, animationCount, level) {
    m_lastTicks = SDL_GetTicks();
}

void Zombie::collideWithPlayer(Player* player) {
    if (player) {
        player->addDamage(static_cast<uint32_t>(Random::getInt(10, 18)));
    }
    prepareDelete();
}

void Zombie::update() {
    Bot::update();
    Bot::run();
    if (m_body->GetLinearVelocity().LengthSquared() < 0.5f) {
        jump();
    }

    Camera& cam = m_level->getCamera();
    Uint32 ticks = SDL_GetTicks();
    if (y() + animationHeight() < cam.minY() || y() > cam.maxY() ||
        x() + animationWidth() < cam.minX() || x() > cam.maxX()) {
        // delete bots after 10 seconds
        if (ticks - m_lastTicks > 10000) {
            prepareDelete();
        }
    } else {
        m_lastTicks = ticks;
    }
}

}  // namespace engine
}  // namespace ctb
