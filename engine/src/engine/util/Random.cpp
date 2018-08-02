// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <random>

#include "engine/util/Random.hpp"

namespace ctb {
namespace engine {

int Random::getInt(int from, int to) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(from, to);
    return dis(gen);
}

bool Random::getBool() {
    return getInt(0, 1) == 1;
}

}  // namespace engine
}  // namespace ctb
