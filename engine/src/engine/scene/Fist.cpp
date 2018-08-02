// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <vector>

#include <gsl/gsl>

#include "engine/Window.hpp"
#include "engine/audio/SoundManager.hpp"
#include "engine/scene/Fist.hpp"

namespace ctb {
namespace engine {

Fist::Fist(SDL_Texture* texture, int animationWidth, int animationHeight, int animationCount)
    : PhysicalRenderable(texture, animationWidth, animationHeight, animationCount, true),
      m_user(nullptr),
      m_tmp_mask(0),
      m_use(false) {}

bool Fist::isDropable() {
    return false;
}

bool Fist::isMelee() {
    return true;
}

void Fist::angleWeapon(double /*angle*/, Direction /*direction*/) {}

void Fist::use() {
    m_use = true;

    if (m_body == nullptr || m_body->GetWorld()->IsLocked()) {
        return;
    }

    b2FixtureDef* tmp = new b2FixtureDef;

    tmp->shape = m_shapeBox;

    //[in kg/m^2]
    tmp->density = m_fixtureDef->density;
    //[0-1]
    tmp->friction = m_fixtureDef->friction;
    //[0-1]
    tmp->restitution = m_fixtureDef->restitution;

    tmp->filter.maskBits = m_tmp_mask;
    tmp->filter.categoryBits = m_fixtureDef->filter.categoryBits;

    m_body->DestroyFixture(m_fixture);
    delete m_fixtureDef;
    m_fixtureDef = tmp;
    m_fixture = m_body->CreateFixture(m_fixtureDef);
}

void Fist::unuse() {
    if (m_body == nullptr || m_body->GetWorld()->IsLocked()) {
        return;
    }

    b2FixtureDef* tmp = new b2FixtureDef;

    if (m_use) {
        m_tmp_mask = m_fixtureDef->filter.maskBits;
    }

    tmp->shape = m_shapeBox;

    //[in kg/m^2]
    tmp->density = m_fixtureDef->density;
    //[0-1]
    tmp->friction = m_fixtureDef->friction;
    //[0-1]
    tmp->restitution = m_fixtureDef->restitution;

    tmp->filter.maskBits = 0;
    tmp->filter.categoryBits = m_fixtureDef->filter.categoryBits;

    m_body->DestroyFixture(m_fixture);
    delete m_fixtureDef;
    m_fixtureDef = tmp;
    m_fixture = m_body->CreateFixture(m_fixtureDef);

    m_use = false;
}

bool Fist::inUse() const {
    return m_use;
}

void Fist::setUser(Player* user) {
    m_user = user;

    m_use = true;
    unuse();
}

Player* Fist::getUser() const {
    return m_user;
}

void Fist::update() {
    PhysicalRenderable::update();
    // m_body->GetWorld()->DrawDebugData();
    if (m_user) {
        align();

        m_body->SetLinearVelocity(m_user->getBody()->GetLinearVelocityFromWorldPoint(
            m_user->getBody()->GetWorldCenter()));
        setWorldPosition(b2Vec2(worldPosition().x, m_user->worldPosition().y));
    } else {
        clearJointList();
    }
}

void Fist::align() {
    if (m_user) {
        clearJointList();

        b2RevoluteJointDef jointDef;
        jointDef.bodyA = m_user->getBody();
        jointDef.bodyB = m_body;
        jointDef.collideConnected = false;

        if (m_user->isMovingRight()) {
            jointDef.localAnchorA.Set(1, 0);
        } else if (m_user->isMovingLeft()) {
            jointDef.localAnchorA.Set(-1, 0);
        } else {
            return;
        }

        m_user->getBody()->GetWorld()->CreateJoint(&jointDef);
    }
}

void Fist::clearJointList() {
    b2JointEdge* edge = m_body->GetJointList();
    while (edge != nullptr) {
        auto* joint = edge->joint;
        edge = edge->next;
        if (joint != nullptr) {
            m_body->GetWorld()->DestroyJoint(joint);
        }
    }
}

Fist::~Fist() {
    if (m_body) {
        for (auto& pair : m_representations) {
            pair.first->DestroyBody(pair.second);
        }
        m_representations.clear();
    }
}

void Fist::addToWorld(b2World& world, Kinematics& kinematics) {
    if (m_fixture == nullptr) {
        PhysicalRenderable::addToWorld(world, kinematics);

        if (m_user) {
            align();
        }
        return;
    }

    m_body->DestroyFixture(m_fixture);
    m_fixture = nullptr;
    PhysicalRenderable::addToWorld(world, kinematics);

    if (m_fixture == nullptr) {
        m_fixture = m_body->CreateFixture(m_fixtureDef);
    }

    if (m_user) {
        align();
    }
}

}  // namespace engine
}  // namespace ctb
