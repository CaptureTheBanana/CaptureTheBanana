// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_PHYSICS_PhysicalObject_HPP
#define ENGINE_PHYSICS_PhysicalObject_HPP

#include <Box2D/Box2D.h>

#include "engine/physics/Kinematics.hpp"

namespace ctb {
namespace engine {

/**
 * @brief Abstract class, which has a representation in a b2World
 */
class PhysicalObject {
   public:
    /**
     * @brief Constructor
     */
    PhysicalObject() : m_collision_id(0) {}

    /**
     * @brief Adds the physical representation of the object to the given b2World
     *
     * @param world to which this object should be added
     * @param kinematics attributes of the physical representation
     */
    virtual void addToWorld(b2World& world, Kinematics& kinematics) = 0;

    /**
     * @brief Sets the collision ID of this object, on which it can be identified in the
     * ContactListener
     *
     * @param id new collision ID of this object
     */
    void setCollisionId(int id);

    /**
     * @brief Gets the collision Id of this object
     *
     * @return the collision ID of this object
     */
    int getCollisionId() const;

    /**
     * @brief Convert the given pixel coordinate to a physical coordinate
     *
     * @param coordinate pixel coordinate, which should be converted
     *
     * @return The converted world coordinate
     */
    static float32 convertToWorldCoordinate(int coordinate);

    /**
     * @brief Convert the given coordinate to a physical coordinate
     *
     * @param coordinate coordinate, which should be converted
     *
     * @return The converted world coordinate
     */
    static float32 convertToWorldCoordinate(double coordinate);

    /**
     * @brief Convert the given physical coordinate to a pixel coordinate
     *
     * @param coordinate coordinate, which should be converted
     *
     * @return The converted pixel coordinate
     */
    static int convertToScreenCoordinate(float32 coordinate);

    /**
     * @brief Destructor
     */
    virtual ~PhysicalObject() = default;

   protected:
    /// From pixel to world coordinates
    static constexpr float32 SCALE = 20.0f;

    /// Can be used, for identify the PhysicalObject in the ContactListener
    int m_collision_id;
};

}  // namespace engine
}  // namespace ctb

#endif
