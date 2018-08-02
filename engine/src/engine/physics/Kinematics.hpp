// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_PHYSICS_KINEMATICS_HPP
#define ENGINE_PHYSICS_KINEMATICS_HPP

#include <stdexcept>

#include <Box2D/Box2D.h>

namespace ctb {
namespace engine {

/**
 * @brief Class for representing the attributes of the physical representation in Box2D
 */
class Kinematics {
   private:
    float32 m_restitution;
    float32 m_friction;
    float32 m_density;
    uint16 m_mask;
    uint16 m_category;
    int m_id;

   public:
    /**
     * @brief Constructor
     *
     * @param res restitution of the representation
     * @param fric friction of the representation
     * @param den  density of the representation
     */
    explicit Kinematics(float32 res = 0.0f, float32 fric = 1.0f, float32 den = 0.0f);

    /**
     * @brief Sets the restitution between 0.0 and 1.0
     *
     * @param res new restitution
     *
     * @throws invalid_argument, if res is not between 0.0 and 1.0
     */
    void setRestitution(float32 res);

    /**
     * @brief Gets the restitution
     *
     * @return a value between 0.0 and 1.0
     */
    float32 getRestitution() const;

    /**
     * @brief Sets the friction between 0.0 and 1.0
     *
     * @param fric new friction
     *
     * @throws invalid_argument if fric is not between 0.0 and 1.0
     */
    void setFriction(float32 fric);

    /**
     * @brief Gets the friction
     *
     * @return a value between 0.0 and 1.0
     */
    float32 getFriction() const;

    /**
     * @brief Sets the density to a value >= 0
     *
     * @param den new density
     *
     * @throws invalid_argument, if den is lower than 0
     */
    void setDensity(float32 den);

    /**
     * @brief Gets the density
     *
     * @return a positive value
     */
    float32 getDensity() const;

    /**
     * @brief Sets the mask, on which PhysicalObject this PhysicalObject will collide with
     *
     * @param mask new mask
     */
    void setMask(uint16 mask);

    /**
     * @brief Gets the mask, on which PhysicalObject this PhysicalObject will collide with
     *
     * @return the mask
     */
    uint16 getMask() const;

    /**
     * @brief Sets the collision category
     *
     * @param category new collision category
     */
    void setCategory(uint16 category);

    /**
     * @brief Gets the collision category
     *
     * @return the collision category
     */
    uint16 getCategory() const;

    /**
     * @brief Sets the ID of the PhysicalObject, which can be used for identify it in the
     * ContactListener
     *
     * @param id new ID
     */
    void setId(int id);

    /**
     * @brief Gets the ID of the PhysicalObject, which can be used for identify it in the
     * ContactListener
     *
     * @return ID of the PhysicalObject
     */
    int getId() const;
};

}  // namespace engine
}  // namespace ctb
#endif
