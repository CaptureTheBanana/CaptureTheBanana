// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/scene/Projectile.hpp"
#include "engine/Window.hpp"
#include "engine/scene/Gun.hpp"

namespace ctb {
namespace engine {

Projectile::Projectile(SDL_Texture* texture,
                       int animationWidth,
                       int animationHeight,
                       int animationCount,
                       Gun* gun,
                       uint32_t damage)
    : PhysicalRenderable(texture, animationWidth, animationHeight, animationCount, true),
      m_gun(gun),
      m_user(nullptr),
      m_damage(damage) {
    if (m_gun) {
        m_user = m_gun->getUser();
    }
}

void Projectile::addToWorld(b2World& world,
                            Kinematics& kinematics,
                            float32 x,
                            float32 y,
                            double angle,
                            float32 power) {
    b2BodyDef circleDef;
    circleDef.type = b2_dynamicBody;
    circleDef.position.Set(x, y /*m_worldPosition.x, m_worldPosition.y*/);
    circleDef.fixedRotation = false;
    b2Body* bullet = world.CreateBody(&circleDef);
    b2CircleShape circleShape;
    circleShape.m_p.Set(0, 0);  // position, relative to body position
    circleShape.m_radius = 1;   // radius

    // create fixture
    b2FixtureDef circleFixture;
    circleFixture.shape = &circleShape;

    //[in kg/m^2]
    circleFixture.density = kinematics.getDensity();
    //[0-1]
    circleFixture.friction = kinematics.getFriction();
    //[0-1]
    circleFixture.restitution = kinematics.getRestitution();

    circleFixture.filter.maskBits = kinematics.getMask();
    circleFixture.filter.categoryBits = kinematics.getCategory();

    bullet->CreateFixture(&circleFixture);
    m_body = bullet;
    m_body->SetBullet(true);
    m_body->SetUserData(this);
    setCollisionId(kinematics.getId());

    // x'=xcosθ-ysinθ  y = 0, x = power
    // y'=xsinθ+ycosθ
    float32 powerX = float32(power * cos(angle));
    float32 powerY = float32(power * sin(-angle));

    b2Vec2 velocity;
    velocity.x = powerX;
    velocity.y = powerY;

    m_body->ApplyLinearImpulse(b2Vec2(powerX * m_body->GetMass(), powerY * m_body->GetMass()),
                               m_body->GetWorldCenter(), true);
}

void Projectile::render() {
    if (m_gun->getAngle() * 180 / M_PI > 90) {
        m_flip = SDL_FLIP_HORIZONTAL;
        m_flip_angle = -m_gun->getAngle() * 180 / M_PI + 180;
    } else {
        m_flip = SDL_FLIP_NONE;
        m_flip_angle = -m_gun->getAngle() * 180 / M_PI;
    }
    PhysicalRenderable::render();
}

void Projectile::update() {
    if (m_body->GetPosition().x < 0 || m_body->GetPosition().y < 0 ||
        m_body->GetPosition().x > 1000 || m_body->GetPosition().y > 1000) {
        m_gun->removeProjectile(this);
    }
    PhysicalRenderable::update();
}

Projectile::~Projectile() {
    if (m_body) {
        m_body->GetWorld()->DestroyBody(m_body);
        m_body = nullptr;
    }
}

}  // namespace engine
}  // namespace ctb
