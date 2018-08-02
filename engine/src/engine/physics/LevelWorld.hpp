// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_PHYSICS_LEVELWORLD_HPP
#define ENGINE_PHYSICS_LEVELWORLD_HPP

#include <Box2D/Box2D.h>

namespace ctb {
namespace engine {

class LevelContactListener;

/// Level world information
class LevelWorld {
   public:
    /// \brief Constructor
    ///
    ///
    /// \param gravity The world gravity vector.
    explicit LevelWorld(const b2Vec2& gravity);

    virtual ~LevelWorld();

    /// Returns the world
    b2World* getWorld() const { return m_world; }

    /// Return the level contact listener
    LevelContactListener* getListener() const { return m_listener; }

   private:
    // Box2d world
    b2World* m_world{nullptr};
    /// the contact listener of this world
    LevelContactListener* m_listener{nullptr};
};

}  // namespace engine
}  // namespace ctb

#endif  // ENGINE_PHYSICS_LEVELWORLD_HPP
