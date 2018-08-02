// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/core/Camera.hpp"
#include "engine/scene/Door.hpp"

namespace ctb {
namespace engine {

Camera::Camera(int x, int y, int w, int h, int levelWidth, int levelHeight)
    : m_position(x, y),
      m_focus(nullptr),
      m_width(w),
      m_height(h),
      m_levelWidth(levelWidth),
      m_levelHeight(levelHeight) {}

void Camera::focusDoor(Door* d) {
    m_focus = nullptr;
    auto pos = d->position();
    m_position.x = pos.x - m_width / 2;
    m_position.y = pos.y;
}

int Camera::x() const {
    return limitValue(m_focus ? m_focus->x() : m_position.x, m_width / 2,
                      m_levelWidth - m_width / 2);
}

int Camera::y() const {
    return limitValue(m_focus ? m_focus->y() : m_position.y, static_cast<int>(m_height / 1.4),
                      m_levelHeight - m_height / 2);
}

bool Camera::checkBounds(TextureBasedRenderable* r) {
    // just check for out of bounds for left, right and bottom.
    return r->y() > maxY() || r->x() + r->width() < minX() || r->x() > maxX();
}

int Camera::limitValue(int val, int min, int max) const {
    if (val < min) {
        return min;
    }
    if (val > max) {
        return max;
    }
    return val;
}

}  // namespace engine
}  // namespace ctb
