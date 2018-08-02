// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/physics/PhysicalObject.hpp"

namespace ctb {
namespace engine {

void PhysicalObject::setCollisionId(int id) {
    m_collision_id = id;
}

int PhysicalObject::getCollisionId() const {
    return m_collision_id;
}

float32 PhysicalObject::convertToWorldCoordinate(int coordinate) {
    return static_cast<float32>(coordinate) / SCALE;  // float32(coordinate) / SCALE;
}

float32 PhysicalObject::convertToWorldCoordinate(double coordinate) {
    return static_cast<float32>(coordinate / SCALE);
}

int PhysicalObject::convertToScreenCoordinate(float32 coordinate) {
    return static_cast<int>(coordinate * SCALE);
}

}  // namespace engine
}  // namespace ctb
