// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_UTIL_COLOR_HPP
#define ENGINE_UTIL_COLOR_HPP

namespace ctb {
namespace engine {

/// A simple helper class to store rgb values
class Color {
   public:
    /// Constructor
    Color() : r(0), g(0), b(0) {}

    /// Constructor with values
    Color(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {}

    /// Copy contructor
    Color(const Color& c) : r(c.r), g(c.g), b(c.b) {}

    /// public red value
    uint8_t r;

    /// public green value
    uint8_t g;

    /// public blue value
    uint8_t b;
};

}  // namespace engine
}  // namespace ctb

#endif
