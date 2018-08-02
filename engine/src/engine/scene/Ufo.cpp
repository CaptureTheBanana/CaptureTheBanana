// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <vector>

#include <SDL.h>

#include "engine/Window.hpp"
#include "engine/core/Camera.hpp"
#include "engine/core/Game.hpp"
#include "engine/scene/Ufo.hpp"
#include "engine/util/Random.hpp"

namespace ctb {
namespace engine {

Ufo::Ufo(SDL_Texture* texture,
         int animationWidth,
         int animationHeight,
         int animationCount,
         Level* level)
    : Bot(texture, animationWidth, animationHeight, animationCount, level) {}

void Ufo::collideWithPlayer(Player* player) {
    if (player) {
        player->addDamage(static_cast<uint32_t>(Random::getInt(12, 45)));
    }
    prepareDelete();
}

void Ufo::update() {
    Bot::update();
    Bot::run();
    m_body->SetGravityScale(0.0f);
}

}  // namespace engine
}  // namespace ctb
