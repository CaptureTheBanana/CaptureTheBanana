// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_SCENE_PROJECTILE_HPP
#define ENGINE_SCENE_PROJECTILE_HPP

#include "engine/graphics/PhysicalRenderable.hpp"

namespace ctb {
namespace engine {

class Gun;

class Player;

/* Abstract projectile class. Need to be derived from,
 * if you want to use it.
 */
class Projectile : public PhysicalRenderable {
   public:
    Projectile(SDL_Texture* texture,
               int animationWidth,
               int animationHeight,
               int animationCount,
               Gun* gun,
               uint32_t damage);

    inline Gun* getGun() const { return m_gun; }

    inline Player* getUser() const { return m_user; }

    using PhysicalRenderable::addToWorld;

    void addToWorld(b2World& world,
                    Kinematics& kinematics,
                    float32 x,
                    float32 y,
                    double angle = 0.0,
                    float32 power = 0.0f);

    void render() override;

    void update() override;

    inline uint32_t getDamage() const { return m_damage; }

    /// Destructor
    ~Projectile() override;

   private:
    Gun* m_gun;

    Player* m_user;

    uint32_t m_damage;
};

}  // namespace engine
}  // namespace ctb

#endif
