// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_PHYSICS_ACTINGKINEMATICS_HPP
#define ENGINE_PHYSICS_ACTINGKINEMATICS_HPP

#include "engine/physics/Kinematics.hpp"

namespace ctb {
namespace engine {

/**
 * @brief Class for representing the attributes of the physical representation, which can be acting,
 * in Box2D
 */
class ActingKinematics : public Kinematics {
   private:
    float32 m_jumpImpulse;
    float32 m_movingForce;

   public:
    /**
     * @brief Constructor
     *
     * @param res restitution of the representation
     * @param fric friction of the representation
     * @param den density of the representation
     * @param jump jumping impulse of the representation
     * @param move moving force of the representation
     */
    explicit ActingKinematics(float32 res = 0.0f,
                              float32 fric = 0.5f,
                              float32 den = 42.0f,
                              float32 jump = 20,
                              float32 move = 850);

    /**
     * @brief Sets the jumping impulse
     *
     * @param impulse of the physical representation
     */
    void setJumpImpulse(float32 impulse);

    /**
     * @brief Gets the jumping impulse
     *
     * @return jumping impulse of the representation
     */
    float32 geJumpImpulse() const;

    /**
     * @brief Sets the moving force
     *
     * @param force of the physical representation
     */
    void setMovingForce(float32 force);

    /**
     * @brief Gets the moving force
     *
     * @return moving force of the representation
     */
    float32 getMovingForce() const;
};

}  // namespace engine
}  // namespace ctb

#endif
