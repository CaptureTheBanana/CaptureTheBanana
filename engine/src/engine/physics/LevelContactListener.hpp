// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_PHYSICS_LEVELCONTACTLISTENER_HPP
#define ENGINE_PHYSICS_LEVELCONTACTLISTENER_HPP

#include <Box2D/Box2D.h>

namespace ctb {
namespace engine {

class Player;

class Bot;

class Door;

class Fist;

class Flag;

class PhysicalObject;

class PhysicalRenderable;

class Projectile;

/**
 * @brief Class, that handles special behavior for certain objects
 */
class LevelContactListener : public b2ContactListener {
   public:
    /**
     * @brief Constructor
     */
    LevelContactListener();

    ~LevelContactListener() override = default;

    /**
     * @brief What should happen at the beginning of a contact of two certain objects?
     *
     * @param contact all necessary contact information
     */
    void BeginContact(b2Contact* contact) override;

    /**
     * @brief What should happen at the end of a contatc of two certain objects?
     *
     * @param contact all necessary contact information
     */
    void EndContact(b2Contact* contact) override;

    /**
     * @brief What should happen before two certain objects are in contact?
     *
     * @param contact all necessary contact information
     * @param oldManifold for two touching convex shapes
     */
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;

    /**
     * @brief Should be called after every step of the b2World.
     *        Does tasks, that cannot be done during the normal contact events,
     *        because in BeginContact, EndContact and PreContact the beWorld is locked
     */
    virtual void update();

   private:
    /**
     * @brief What should happen, if an player reaches the right door with the banana?
     *
     * @param contact all necessary contact information
     * @param player who reached the door
     * @param door which was reached
     */
    void doorReached(b2Contact* contact, Player* player, Door* door);

    /**
     * @brief Makes, that the given player is the owner of the given flag
     *
     * @param player who whould be the owner of the flag
     * @param flag who should be owned by the player
     */
    void flagOwned(Player* player, Flag* flag);

    /**
     * @brief Proves, if the given flag is owned by an player
     *
     * @param obj must be a flag
     *
     * @return Is the flag in use?
     */
    bool isFlagInUse(PhysicalObject* obj);

    /**
     * @brief What should happen, if an player collides with a bot?
     *
     * @param player who is colliding
     * @param bot who is colliding
     */
    void collidedBotPlayer(Player* player, Bot* bot);

    /**
     * @brief Perform a melee attack on an other player with a cooldown
     *
     * @param attacking the attacking player
     * @param hurt the player, who is attacked
     */
    void meleeWithCooldown(Player* attacking, Player* hurt);

    /**
     * @brief What should happen, if an player collects a flag
     *
     * @param player who is colliding with a flag
     * @param flag which is colliding with an player
     */
    void collectFlag(Player* player, Flag* flag);

    /**
     * @brief What should happen, if an player collides with a weapon
     *
     * @param player who is colliding with a weapon
     * @param weapon which is colliding with an player
     * @param contact information about the collision
     */
    void collisionPlayerWeapon(Player* player, Fist* weapon, b2Contact* contact);

    /**
     * @brief What should happen, if a projectile collides with an other PhysicalObject
     *
     * @param projectile which is colliding with a PhysicalObject
     * @param obj which is colliding with a projectile
     * @param contact information about the collision
     */
    void collisionWithProjectile(Projectile* projectile, PhysicalObject* obj, b2Contact* contact);

    /**
     * @brief Method for ignoring the contact betwen a door and an player
     *
     * @param door which is colliding with an player
     * @param player which is colliding with a door
     * @param contact information about the collision
     */
    void doorIgnoring(Door* door, Player* player, b2Contact* contact);

    /// Reference to the first object, which is collided for the update method
    PhysicalRenderable* m_a;
    /// Reference to the second object, which is collided for the update method
    PhysicalRenderable* m_b;

    /// Reference to an player for the update method
    Player* m_player;
};

}  // namespace engine
}  // namespace ctb

#endif
