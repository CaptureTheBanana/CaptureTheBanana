// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/physics/LevelWorld.hpp"
#include "engine/physics/LevelContactListener.hpp"

namespace ctb {
namespace engine {

LevelWorld::LevelWorld(const b2Vec2& gravity) {
    m_listener = new LevelContactListener;
    m_world = new b2World(gravity);
    m_world->SetContactListener(m_listener);
}

LevelWorld::~LevelWorld() {
    delete m_listener;
    delete m_world;
}

}  // namespace engine
}  // namespace ctb
