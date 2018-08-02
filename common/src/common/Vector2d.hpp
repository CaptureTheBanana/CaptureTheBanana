// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef COMMON_VECTOR2D_HPP
#define COMMON_VECTOR2D_HPP

namespace ctb {
namespace common {

/// 2D vector
struct Vector2d {
    int x{-1};
    int y{-1};

    Vector2d() = delete;
    Vector2d(int _x, int _y) : x(_x), y(_y) {}

    /// Returns true if x and y >= 0.
    bool isValid() { return x >= 0 && y >= 0; }

    bool operator==(const Vector2d& other) const { return other.x == x && other.y == y; }

    bool operator!=(const Vector2d& other) const { return !(*this == other); }

    Vector2d operator+(const Vector2d& other) const {
        Vector2d tmp = *this;  // copy
        tmp += other;
        return tmp;
    }

    Vector2d operator-(const Vector2d& other) const {
        Vector2d tmp = *this;  // copy
        tmp -= other;
        return tmp;
    }

    Vector2d& operator+=(const Vector2d& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2d& operator-=(const Vector2d& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
};

}  // namespace common
}  // namespace ctb

#endif  // COMMON_VECTOR2D_HPP
