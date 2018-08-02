// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/graphics/PhysicalRenderable.hpp"
#include "engine/Window.hpp"
#include "engine/core/Game.hpp"
#include "engine/util/Vector2d.hpp"

namespace ctb {
namespace engine {

PhysicalRenderable::PhysicalRenderable(SDL_Texture* animations,
                                       int animationWidth,
                                       int animationHeight,
                                       int animationCount,
                                       bool dynamic)
    : TextureBasedRenderable(animations),
      m_animationCount(animationCount),
      m_currentAnimationStep(0),
      m_animationHeight(animationHeight),
      m_animationWidth(animationWidth),
      m_animationDuration(0),
      m_lastTick(0),
      m_body(nullptr),
      m_joint(nullptr),
      m_fixtureDef(nullptr),
      m_shapeBox(nullptr),
      m_fixture(nullptr),
      m_dynamic(dynamic),
      m_scaleX(1.0f),
      m_scaleY(1.0f) {
    m_sourceRect.w = animationWidth;
    m_sourceRect.h = animationHeight;
    m_targetRect.w = animationWidth;
    m_targetRect.h = animationHeight;
    computeWorldCoordinates();
}

PhysicalRenderable::~PhysicalRenderable() {
    // removes and destroyes the body from the world
    if (m_fixtureDef) {
        delete m_fixtureDef;
        m_fixtureDef = nullptr;
    }

    if (m_shapeBox) {
        delete m_shapeBox;
        m_shapeBox = nullptr;
    }
}

void PhysicalRenderable::nextAnimation() {
    Uint32 ticks = SDL_GetTicks();
    if (m_animationDuration < (ticks - m_lastTick)) {
        // Set next animation step
        m_currentAnimationStep++;
        if (m_currentAnimationStep >= m_animationCount) {
            m_currentAnimationStep = 0;
        }

        m_sourceRect.x = m_currentAnimationStep * m_animationWidth;
        m_lastTick = ticks;
    }
}

void PhysicalRenderable::reset() {
    m_body->SetLinearVelocity(b2Vec2(0, 0));
}

void PhysicalRenderable::setFPS(int frames) {
    m_animationDuration = static_cast<Uint32>(1000.0 / static_cast<double>(frames));
}

void PhysicalRenderable::setWorldPosition(const b2Vec2& position) {
    m_worldPosition = position;
    m_body->SetTransform(position, 0);
    computeScreenCoordinates();
}

b2Vec2 PhysicalRenderable::worldPosition() const {
    return m_worldPosition;
}

void PhysicalRenderable::setPosition(const Vector2dT& vector) {
    TextureBasedRenderable::setPosition(vector);
    computeWorldCoordinates();
}

void PhysicalRenderable::createJoint(PhysicalRenderable* other) {
    b2RevoluteJointDef jointDef;
    jointDef.bodyA = m_body;
    jointDef.bodyB = other->getBody();
    jointDef.collideConnected = false;
    jointDef.localAnchorA.Set(0, 2);
    m_joint = other->m_joint = m_body->GetWorld()->CreateJoint(&jointDef);
}

void PhysicalRenderable::destroyJoint() {
    if (m_joint) {
        auto* j = m_joint;
        PhysicalRenderable* a = static_cast<PhysicalRenderable*>(j->GetBodyA()->GetUserData());
        PhysicalRenderable* b = static_cast<PhysicalRenderable*>(j->GetBodyB()->GetUserData());
        a->m_joint = nullptr;
        b->m_joint = nullptr;

        // Perform a little jump by the banana
        if (a->getCollisionId() == Game::FLAG_ID) {
            j->GetBodyA()->SetLinearVelocity(b2Vec2(0, -15));
        } else if (b->getCollisionId() == Game::FLAG_ID) {
            j->GetBodyB()->SetLinearVelocity(b2Vec2(0, -15));
        }

        m_body->GetWorld()->DestroyJoint(j);
    }
}

void PhysicalRenderable::render() {
    if (m_texture != nullptr) {
        nextAnimation();
        Vector2dT position = computeTargetPosition();
        SDL_Rect animationRect;
        animationRect.x = position.x;
        animationRect.y = position.y;
        animationRect.w = m_targetRect.w;
        animationRect.h = m_targetRect.h;

        SDL_RenderCopyEx(Window::getWindow().renderer(), m_texture, &m_sourceRect, &animationRect,
                         m_flip_angle, nullptr, m_flip);
    }
}

void PhysicalRenderable::addToWorld(b2World& world, Kinematics& kinematics) {
    // Is there a physical representation for this object in the given world?
    for (auto& pair : m_representations) {
        if (pair.first == &world) {
            m_body = pair.second;
            return;
        }
    }

    b2BodyDef boxdef;
    if (m_dynamic) {
        boxdef.type = b2_dynamicBody;
    } else {
        boxdef.type = b2_staticBody;
    }

    boxdef.position.Set(m_worldPosition.x, m_worldPosition.y);

    boxdef.fixedRotation = true;

    // second step create body
    b2Body* body = world.CreateBody(&boxdef);

    // third step create shape
    delete m_shapeBox;
    m_shapeBox = new b2PolygonShape;
    m_shapeBox->SetAsBox(convertToWorldCoordinate(m_animationWidth / 2),
                         convertToWorldCoordinate(m_animationHeight / 2));

    // create fixture

    delete m_fixtureDef;
    m_fixtureDef = new b2FixtureDef;

    m_fixtureDef->shape = m_shapeBox;

    //[in kg/m^2]
    m_fixtureDef->density = kinematics.getDensity();
    //[0-1]
    m_fixtureDef->friction = kinematics.getFriction();
    //[0-1]
    m_fixtureDef->restitution = kinematics.getRestitution();

    // Sets the collision behavior
    m_fixtureDef->filter.maskBits = kinematics.getMask();
    m_fixtureDef->filter.categoryBits = kinematics.getCategory();

    m_fixture = body->CreateFixture(m_fixtureDef);

    m_body = body;
    m_body->SetBullet(false);
    m_body->SetUserData(this);
    setCollisionId(kinematics.getId());

    // Creates a new representation entry
    m_representations.emplace_back(&world, m_body);
}

void PhysicalRenderable::computeScreenCoordinates() {
    // Round world coordinates to nearest int
    m_targetRect.x = convertToScreenCoordinate(m_worldPosition.x) - m_animationWidth / 2;
    m_targetRect.y = convertToScreenCoordinate(m_worldPosition.y) - m_animationHeight / 2;

    // Copy screen coordinates to position vector
    m_position.x = m_targetRect.x;
    m_position.y = m_targetRect.y;
}

void PhysicalRenderable::computeWorldCoordinates() {
    m_worldPosition.x = convertToWorldCoordinate(m_targetRect.x + m_animationWidth / 2);
    m_worldPosition.y = convertToWorldCoordinate(m_targetRect.y + m_animationHeight / 2);

    // Actualize world coordinates
    if (m_body) {
        m_body->SetTransform(m_worldPosition, 0);
    }
}

void PhysicalRenderable::update() {
    setWorldPosition(m_body->GetPosition());
}

}  // namespace engine
}  // namespace ctb
