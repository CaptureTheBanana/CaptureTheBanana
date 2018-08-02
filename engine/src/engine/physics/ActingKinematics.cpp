// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/physics/ActingKinematics.hpp"

namespace ctb {
namespace engine {

ActingKinematics::ActingKinematics(float32 res,
                                   float32 fric,
                                   float32 den,
                                   float32 jump,
                                   float32 move)
    : Kinematics(res, fric, den) {
    setMovingForce(move);
    setJumpImpulse(jump);
}

void ActingKinematics::setJumpImpulse(float32 impulse) {
    if (impulse < 0.0f) {
        throw std::invalid_argument("jump impulse must be grater than 0");
    } else {
        m_jumpImpulse = impulse;
    }
}

float32 ActingKinematics::geJumpImpulse() const {
    return m_jumpImpulse;
}

void ActingKinematics::setMovingForce(float32 force) {
    if (force < 0.0f) {
        throw std::invalid_argument("moving impulse must be an absolute value");
    } else {
        m_movingForce = force;
    }
}

float32 ActingKinematics::getMovingForce() const {
    return m_movingForce;
}

}  // namespace engine
}  // namespace ctb
