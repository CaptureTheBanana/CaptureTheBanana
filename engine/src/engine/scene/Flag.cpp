// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/scene/Flag.hpp"
#include "engine/Window.hpp"
#include "engine/core/Game.hpp"
#include "engine/core/Level.hpp"

namespace ctb {
namespace engine {

Flag::Flag(SDL_Texture* texture, int animationWidth, int animationHeight, int animationCount)
    : PhysicalRenderable(texture, animationWidth, animationHeight, animationCount, true) {}

void Flag::addToThisWorld(b2World& world) {
    Kinematics k;
    k.setCategory(Level::FLAG_CAT);
    k.setMask(Level::PLAYER_CAT | Level::GROUND_CAT);
    k.setId(Game::FLAG_ID);
    PhysicalRenderable::addToWorld(world, k);
}

}  // namespace engine
}  // namespace ctb
