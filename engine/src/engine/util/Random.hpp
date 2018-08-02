// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_UTIL_RANDOM_HPP
#define ENGINE_UTIL_RANDOM_HPP

namespace ctb {
namespace engine {

class Random {
   public:
    /// returns a random int in [from, to]
    static int getInt(int from, int to);

    /// returns a random true or false
    static bool getBool();
};

}  // namespace engine
}  // namespace ctb

#endif  // ENGINE_UTIL_RANDOM_HPP
