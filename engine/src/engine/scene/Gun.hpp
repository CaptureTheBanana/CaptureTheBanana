// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

// TODO(felix): Put magazine capacity and reload time into game.xml file
// TODO(felix): Show "no ammunition" message on screen

#ifndef ENGINE_SCENE_GUN_HPP
#define ENGINE_SCENE_GUN_HPP

#include <string>
#include <vector>

#include <SDL.h>

#include "engine/scene/Fist.hpp"

namespace ctb {
namespace engine {

class Projectile;

/**
 * @brief class, which represents a weapon, which shoots projectiles
 */
class Gun : public Fist {
   public:
    /**
     * @brief Constructor
     *
     * @param texture which should be rendered
     * @param animationWidth of the texture
     * @param animationHeight of the texture
     * @param animationCount of the texture
     * @param projectileTexturePath Path to texture of the projectile, that should be shot
     * @param projectileAnimationHeight frame-height of the projectile texture
     * @param projectileAnimationWidth frame-width of the projectile texture
     * @param cooldown sleep interval for the shooting rate
     * @param projectileSpeed speed of the shot projectile
     * @param projectileDamage damage, that should be produced by a shot projectile
     */
    Gun(SDL_Texture* texture,
        int animationWidth,
        int animationHeight,
        int animationCount,
        std::string projectileTexturePath,
        int projectileAnimationHeight,
        int projectileAnimationWidth,
        int projectileAnimationCount,
        int cooldown,
        float projectileSpeed,
        uint32_t projectileDamage);

    /**
     * @brief Returns, if this weapon is dropable
     *
     * @return Is this weapon dropable?
     */
    bool isDropable() override;

    /**
     * @brief Returns, if this weapon is for melee
     *
     * @return Is this weapon for melee?
     */
    bool isMelee() override;

    /**
     * @brief Starrt using this weapon
     */
    void use() override;

    /**
     * @brief Stop using this weapon
     */
    void unuse() override;

    /**
     * @brief Updates the position of this weapon, which depends on the physical environment
     */
    void update() override;

    /**
     * @brief Render the texture of this weapon
     */
    void render() override;

    /**
     * @brief Drops this weapon
     *
     * @param xVelocity with a velocity in x-direction
     */
    void drop(bool xVelocity) override;

    /**
     * @brief Sets the angle of this weapon
     *
     * @param angle of this weapon
     * @param direction of this weapon
     */
    void angleWeapon(double angle, Direction direction) override;

    /// Returns  the angle of this weapon
    virtual double getAngle() { return m_angle; }

    /**
     * @brief Removes a given projectile, which was shot by this weapon
     *
     * @param projectile which should be removed.
     *                   It must be shot by this weapon
     */
    void removeProjectile(Projectile* projectile);

    /**
     * @brief Removes all projectiles, which were shot by this gun
     */
    void removeAllProjectiles();

    /**
     * @brief Destructor
     */
    ~Gun() override;

   private:
    /// Magazine capacity
    static constexpr size_t kMagazineCapacity{30};

    /// Magazine reload delay in ms
    static constexpr double kReloadDelay{1500};

    /// Maximal magazine count
    static constexpr size_t kMagazineCount{7};

    /// List of projectiles, which were shot by this gun
    std::vector<Projectile*> m_projectiles;

    /// List of projectiles, which should be deleted in the next update call
    std::vector<Projectile*> m_deleteProjectiles;

    /// Path to the texture for the projetiles, which were shot by this gun
    std::string m_projectileTexturePath;

    /// Height of the texture
    int m_projectileFrameHeight;

    /// Width of the texture
    int m_projectileFrameWidth;

    /// Number of frames
    int m_projectileNumFrames;

    /// Angle of the gun
    double m_angle;

    /// cooldown for the next shot
    int m_cooldown;

    /// Speed of the projectile
    float m_projectileSpeed;

    /// Time of the last shot
    int m_lastShot{0};

    /// Damage, caused by this weapon
    uint32_t m_projectileDamage;

    /// Current ammunition count - reload on first pick up
    size_t m_ammo{0};

    /// Magazine reload count
    size_t m_reloadCount{0};

    /// Magazine reload start time
    int m_reloadStartTime{0};
};

}  // namespace engine
}  // namespace ctb

#endif
