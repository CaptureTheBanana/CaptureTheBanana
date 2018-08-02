// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/physics/Kinematics.hpp"

namespace ctb {
namespace engine {

Kinematics::Kinematics(float32 res, float32 fric, float32 den)
    : m_mask(65535), m_category(1), m_id(0) {
    setRestitution(res);
    setFriction(fric);
    setDensity(den);
}

void Kinematics::setRestitution(float32 res) {
    if (res < 0.0f || res > 1.0f) {
        throw std::invalid_argument(
            "restitution must be in range of 0...1\n0->no restitution\n1->max restitution");
    } else {
        m_restitution = res;
    }
}

float32 Kinematics::getRestitution() const {
    return m_restitution;
}

void Kinematics::setFriction(float32 fric) {
    if (fric < 0.0f || fric > 1.0f) {
        throw std::invalid_argument(
            "friction must be in range of 0...1\n0->no friction\n1->max friction");
    } else {
        m_friction = fric;
    }
}

float32 Kinematics::getFriction() const {
    return m_friction;
}

void Kinematics::setDensity(const float32 den) {
    if (den < 0) {
        throw std::invalid_argument(
            "density has to be positiv, because the are no negativ wheight Objects (even if "
            "theoretically posiible ;) )");
    } else {
        m_density = den;
    }
}

float32 Kinematics::getDensity() const {
    return m_density;
}

void Kinematics::setMask(const uint16 mask) {
    m_mask = mask;
}

uint16 Kinematics::getMask() const {
    return m_mask;
}

void Kinematics::setCategory(const uint16 category) {
    m_category = category;
}

uint16 Kinematics::getCategory() const {
    return m_category;
}

void Kinematics::setId(const int id) {
    m_id = id;
}

int Kinematics::getId() const {
    return m_id;
}

}  // namespace engine
}  // namespace ctb
