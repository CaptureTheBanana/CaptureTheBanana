// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/graphics/ActingRenderable.hpp"
#include "engine/core/Level.hpp"
#include "engine/physics/ActingKinematics.hpp"

namespace ctb {
namespace engine {

ActingRenderable::ActingRenderable(SDL_Texture* texture,
                                   int animationWidth,
                                   int animationHeight,
                                   int animationCount)
    : PhysicalRenderable(texture, animationWidth, animationHeight, animationCount, true),
      m_moveright(true),
      m_force(0),
      m_impulse(0),
      m_ground_counter(0) {}

void ActingRenderable::addToWorld(b2World& world, ActingKinematics& kinematics) {
    PhysicalRenderable::addToWorld(world, kinematics);
    m_body->SetUserData(this);

    m_impulse = kinematics.geJumpImpulse();
    m_force = kinematics.getMovingForce();
}

bool ActingRenderable::onGround() const {
    return m_ground_counter != 0;
}

void ActingRenderable::setOnGround(bool onGround) {
    // Mechanism, if the ActingRenderable is between two ground shapes

    if (onGround) {
        m_ground_counter++;
    } else if (m_ground_counter > 0) {
        m_ground_counter--;
    }
}

void ActingRenderable::resetOnGround() {
    m_ground_counter = 0;
}

void ActingRenderable::jump() {
    if (m_ground_counter) {
        m_body->ApplyLinearImpulse(b2Vec2(0, -1 * m_impulse * m_body->GetMass()),
                                   m_body->GetWorldCenter(), true);
    }
}

void ActingRenderable::moveRight() {
    move(true);
    m_moveright = true;
}

void ActingRenderable::moveLeft() {
    move(false);
    m_moveright = false;
}

void ActingRenderable::move(bool moveRight) {
    if (moveRight && m_body->GetLinearVelocity().x >= 0) {
        m_body->ApplyForceToCenter(
            b2Vec2((-m_body->GetLinearVelocity().x + 15) / 7.5f * m_force * m_body->GetMass() /
                       /*40.0f*/ Level::PHYSICALTIMESTEPFREQUENCE * (1.0f - !onGround() * 0.5f),
                   0.0),
            true);
    } else if (moveRight && m_body->GetLinearVelocity().x < 0) {
        m_body->ApplyForceToCenter(
            b2Vec2((1 + (-m_body->GetLinearVelocity().x + 15) / 7.5f) * m_force *
                       m_body->GetMass() / /*40.0f*/ Level::PHYSICALTIMESTEPFREQUENCE *
                       (1.0f - !onGround() * 0.5f),
                   0.0),
            true);
    } else if (!moveRight && m_body->GetLinearVelocity().x <= 0) {
        m_body->ApplyForceToCenter(
            // scaling max speed (Impulse decreases linear)
            b2Vec2(-(m_body->GetLinearVelocity().x + 15) / 7.5f * m_force * m_body->GetMass() /
                       /*40.0f*/ Level::PHYSICALTIMESTEPFREQUENCE * (1.0f - !onGround() * 0.5f),
                   0.0),
            true);
    } else {
        m_body->ApplyForceToCenter(
            b2Vec2((1 + (m_body->GetLinearVelocity().x + 15) / 7.5f) * -m_force *
                       m_body->GetMass() / /*40.0f*/ Level::PHYSICALTIMESTEPFREQUENCE *
                       (1.0f - !onGround() * 0.5f),
                   0.0),
            true);
    }
}

}  // namespace engine
}  // namespace ctb
