// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_SCENE_FIST_HPP
#define ENGINE_SCENE_FIST_HPP

#include "engine/graphics/PhysicalRenderable.hpp"
#include "engine/scene/Player.hpp"
#include "engine/scene/Weapon.hpp"

namespace ctb {
namespace engine {

/*
 * @brief Class, that represents a fist, that is the default weapon of our players
 */
class Fist : public Weapon, public PhysicalRenderable {
   protected:
    /// User of this weapon
    Player* m_user{nullptr};

    /// Temporary collsion mask
    uint16 m_tmp_mask;

    /// Is this weapon in use?
    bool m_use;

    /**
     * @brief Align the weapon to the user
     */
    void align();

    /**
     * @brief Destroy every joint with this weapon
     */
    void clearJointList();

   public:
    /**
     * @brief Constructor
     *
     * @param texture which should be rendered
     * @param animationWidth of the texture
     * @param animationHeight of the texture
     * @param animationCount of the texture
     */
    Fist(SDL_Texture* texture, int animationWidth, int animationHeight, int animationCount);

    /**
     * @brief Start using this weapon
     */
    void use() override;

    /**
     * @brief Stop using this weapon
     */
    void unuse() override;

    /**
     * @brief returns, if this weapon is in using
     *
     * @return is this weapon in using?
     */
    bool inUse() const override;

    /**
     * @brief Sets the user of this weapon.
     *        If user is nullptr, this user has no user
     *
     * @param user new user of this weapon
     */
    void setUser(Player* user) override;

    /**
     * @brief Returns the user of this weapon
     *
     * @return the user of this weapon.
     *         If nullptr, this weapon has no user
     */
    Player* getUser() const override;

    /**
     * @brief Updates the position of this player, that depends on the physical environment
     */
    void update() override;

    /**
     * @brief Returns, if this weapon can be dropped
     *
     * @return Can this weapon be dropped?
     */
    bool isDropable() override;

    /**
     * @brief Returns, if this weapon is for melee
     *
     * @return Is this weapon for melee?
     */
    bool isMelee() override;

    /**
     * @brief This function does nothing, because a fist cannot be dropped
     *
     * @param xVelocity with a velocity in x-direction?
     */
    void drop(bool /*xVelocity*/) override {}

    /**
     * @brief Sets the angle of the weapon
     *
     * @param angle of the weapon
     * @param direction of the weapon
     */
    void angleWeapon(double angle, Direction direction) override;

    /**
     * @brief Adds a physical representation of this weapon to the given b2World
     *
     * @param world to which the physical representation should be added
     * @param kinematics attributes for the physical representation
     */
    void addToWorld(b2World& world, Kinematics& kinematics) override;

    /**
     * @brief Destructor
     */
    ~Fist() override;
};

}  // namespace engine
}  // namespace ctb

#endif
