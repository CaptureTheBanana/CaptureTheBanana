// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_SCENE_ZOMBIE_HPP
#define ENGINE_SCENE_ZOMBIE_HPP

#include <SDL.h>

#include "engine/physics/ActingKinematics.hpp"
#include "engine/scene/Bot.hpp"
#include "engine/scene/Player.hpp"

namespace ctb {
namespace engine {

class Player;

/**
 * @brief	A class to represent an object, that other objects
 * 			can collide with, but can not be moved
 */
class Zombie : public Bot {
   public:
    /// Creates a non-playble character from a texture and animation information
    Zombie(SDL_Texture* texture,
           int animationWidth,
           int animationHeight,
           int animationCount,
           Level* level);

    void update() override;

    void collideWithPlayer(Player* player) override;

    void addToThisWorld(b2World& world);

    /// Destructor
    virtual ~Zombie() {}

   private:
    Uint32 m_lastTicks;
};

}  // namespace engine
}  // namespace ctb

#endif
