// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_SCENE_WEAPON_HPP
#define ENGINE_SCENE_WEAPON_HPP

namespace ctb {
namespace engine {

enum class Direction;

class Player;

/**
 * @brief Interface for representing a weapon
 */
class Weapon {
   public:
    /**
     * @brief Returns, if this weapon is dropabel
     *
     * @return Is this weapon dropable?
     */
    virtual bool isDropable() = 0;

    /**
     * @brief Returns, if this weapon is for melee
     *
     * @return Is this weapon for melee?
     */
    virtual bool isMelee() = 0;

    /**
     * @brief Starrt using this weapon
     */
    virtual void use() = 0;

    /**
     * @brief Stop using this weapon
     */
    virtual void unuse() = 0;

    /**
     * @brief Returns, if this weapon is in using
     *
     * @return Is this weapon in using?
     */
    virtual bool inUse() const = 0;

    /**
     * @brief Sets the user of this weapon.
     *        If the user is nullptr, this weapon has no weapon
     *
     * @param user new user of this weapon
     */
    virtual void setUser(Player* user) = 0;

    /**
     * @brief Returns the user of this weapon.
     *
     * @return the user of this weapon.
     *         If the return value is nullptr,
     *         the weapon has no user
     */
    virtual Player* getUser() const = 0;

    /**
     * @brief Drops this weapon
     *
     * @param xVelocity with a velocity in x-direction?
     */
    virtual void drop(bool xVelocity) = 0;

    /**
     * @brief Destructor
     */
    virtual ~Weapon() = default;

    /**
     * @brief Sets the angle of this weapon
     *
     * @param angle of this weapon
     * @param direction of this weapon
     */
    virtual void angleWeapon(double angle, Direction direction) = 0;
};

}  // namespace engine
}  // namespace ctb

#endif
