// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/scene/Door.hpp"
#include "engine/Window.hpp"
#include "engine/core/Game.hpp"

namespace ctb {
namespace engine {

Door::Door(Team team,
           SDL_Texture* texture,
           int animationWidth,
           int animationHeight,
           int animationCount,
           int offset)
    : PhysicalRenderable(texture, animationWidth, animationHeight, animationCount, false),
      m_team(team),
      m_offset(offset) {
    // scaling for the physical body so its smaller than the whole door texture
    m_scaleX = 0.001f;
    m_scaleY = 0.6f;
}

void Door::addToThisWorld(b2World& world) {
    Kinematics k;
    k.setCategory(Level::DOOR_CAT);
    k.setMask(Level::PLAYER_CAT | Level::GROUND_CAT);
    k.setId(Game::DOOR_ID);
    k.setDensity(10000000);  // high density so it wont move
    PhysicalRenderable::addToWorld(world, k);
}

void Door::setPosition(const Vector2dT& v) {
    PhysicalRenderable::setPosition(Vector2dT(v.x, v.y - m_offset));
}

}  // namespace engine
}  // namespace ctb
