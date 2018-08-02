#ifndef ENGINE_GRAPHIC_PHYSICALRENDERABLE_HPP
#define ENGINE_GRAPHIC_PHYSICALRENDERABLE_HPP
#include <string>
#include <vector>
// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <Box2D/Box2D.h>
#include <SDL.h>

#include "engine/graphics/TextureBasedRenderable.hpp"
#include "engine/physics/Kinematics.hpp"
#include "engine/physics/PhysicalObject.hpp"

namespace ctb {
namespace engine {

class IWindowWithEngine;

/**
 * @brief Class that handles rendering of animations with a physical base.
 */
class PhysicalRenderable : public PhysicalObject, public TextureBasedRenderable {
   public:
    /**
     * @brief Constructor
     *
     * @param animations animation texture to render
     * @param frameWidth of the texture
     * @param frameHeight of the texture
     * @param numFrames of the texture
     * @param dynamic is this object affected by forces?
     */
    PhysicalRenderable(SDL_Texture* animations,
                       int animationWidth,
                       int animationHeight,
                       int animationCount,
                       bool dynamic);

    /**
     * @brief Destructor.
     */
    ~PhysicalRenderable() override;

    /**
     * @brief Renders the current animation step
     */
    void render() override;

    /**
     * @brief Sets the next animation step
     */
    void nextAnimation();

    /**
     * @brief Sets FPS lock
     */
    void setFPS(int frames);

    /**
     * @brief Sets the current position in world coordinates.
     *        The world position is synchronous to the screen position
     *
     * @param position in the b2World
     */
    void setWorldPosition(const b2Vec2& position);

    /**
     * @brief Returns the current position in the b2World
     *
     * @return the position in the b2World
     */
    b2Vec2 worldPosition() const;

    /**
     * @brief update the position, that depends on the physical environment, of this object
     */
    virtual void update();

    /**
     * @brief Sets the screen position of this object.
     *        The screen position is synchronous to the world position
     *
     * @param vector position on the screen
     */
    void setPosition(const Vector2dT& vector) override;

    /**
     * @brief Adds the physical representation of the object to the given b2World
     *
     * @param world to which this object should be added
     * @param kinematics attributes of the physical representation
     */
    void addToWorld(b2World& world, Kinematics& kinematics) override;

    /**
     * @brief Return the animation width of this object
     *
     * @return the animation width of this object
     */
    int animationWidth() const { return m_animationWidth; }

    /**
     * @brief Returns the animation height of this object
     *
     * @return the animation height of this object
     */
    int animationHeight() const { return m_animationHeight; }

    /**
     * @brief Returns the physical representation of this object
     *
     * @return a b2Body, which represents this object in a b2World
     */
    b2Body* getBody() const { return m_body; }

    /**
     * @brief Creates a joint with an other PhysicalRenderable
     *
     * @param other PhysicalRenderable, which should be connected with this object
     */
    void createJoint(PhysicalRenderable* other);

    /**
     * @brief Destroys the current joint with this object
     */
    void destroyJoint();

    /**
     * @brief Resets the velocity of this object
     */
    virtual void reset();

   protected:
    /// Number of animations
    int m_animationCount;

    /// Current animation index
    int m_currentAnimationStep;

    /// Height of a animation
    int m_animationHeight;

    /// Width of a animation
    int m_animationWidth;

    /// Duration between two animations
    Uint32 m_animationDuration;

    /// Last render tick
    Uint32 m_lastTick;

    /// Position of the renderable in world coordinates
    b2Vec2 m_worldPosition;

    /// Physical representation
    b2Body* m_body;

    /// Banana joint
    b2Joint* m_joint;

    /// Fixture definition
    b2FixtureDef* m_fixtureDef;

    /// Shape for the physical representation
    b2PolygonShape* m_shapeBox;

    /// Fixture for the physical representation
    b2Fixture* m_fixture;

    /// Is this object affected by forces of the world?
    bool m_dynamic;

    float m_scaleX;
    float m_scaleY;

    /// List of all physical representations in different worlds for this object
    std::vector<std::pair<b2World*, b2Body*>> m_representations;

    /// Compute screen coordinates from world coordinates
    void computeScreenCoordinates();

    /// Compute world coordinates from screen coordinates
    void computeWorldCoordinates();
};

}  // namespace engine
}  // namespace ctb

#endif  // ENGINE_GRAPHIC_PHYSICALRENDERABLE_HPP
