// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_GRAPHIC_ActingRenderable_HPP
#define ENGINE_GRAPHIC_ActingRenderable_HPP

#include <Box2D/Box2D.h>
#include <SDL.h>

#include "engine/graphics/PhysicalRenderable.hpp"

namespace ctb {
namespace engine {

class ActingKinematics;

/**
 * @brief Class, that represents a DynamicRenderable, that can move and jump
 */
class ActingRenderable : public PhysicalRenderable {
   public:
    /**
     * @brief Constructor
     *
     * @param texture which should be rendered
     * @param animationWidth of the texture
     * @param animationHeight of the texture
     * @param animationCount of the texture
     */
    ActingRenderable(SDL_Texture* texture,
                     int animationWidth,
                     int animationHeight,
                     int animationCount);

    /// Destructor
    ~ActingRenderable() override = default;

    using PhysicalRenderable::addToWorld;

    /**
     * @brief Adds the physical representation of the object to the given b2World
     *
     * @param world to which this object should be added
     * @param kinematics attributes of the physical representation
     */
    virtual void addToWorld(b2World& world, ActingKinematics& kinematics);

    /**
     * @brief Returns, if the object is on the ground
     *
     * @return is this object on the ground?
     */
    bool onGround() const;

    /**
     * @brief Sets the on ground flag to the given status
     *
     * @param onGround is the object on the ground?
     */
    void setOnGround(bool onGround);

    /**
     * @brief Reset the counter for the ground collisions of this object
     */
    void resetOnGround();

    /**
     * @brief Let this object perform a jump, if it is on the ground
     */
    virtual void jump();

    /**
     * @brief let this object move to the right
     */
    void moveRight();

    /**
     * @brief Let this object move to the left
     */
    void moveLeft();

   protected:
    /**
     * @brief Let this object move in the given direction
     *
     * @param moveRight should this object move to the right?
     */
    void move(bool moveRight);

    /// True if the player moves to the right
    bool m_moveright;

    /// Moving force
    float32 m_force;

    /// Moving impulse
    float32 m_impulse;

    /// Count of the ground collisions
    int m_ground_counter;
};

}  // namespace engine
}  // namespace ctb

#endif
