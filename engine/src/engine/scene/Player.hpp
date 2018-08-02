// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_SCENE_PLAYER_HPP
#define ENGINE_SCENE_PLAYER_HPP

#include <iostream>
#include <string>

#include "engine/graphics/ActingRenderable.hpp"
#include "engine/input/Input.hpp"
#include "engine/menu/PauseMenu.hpp"
#include "engine/physics/ActingKinematics.hpp"
#include "engine/util/Vector2d.hpp"

namespace ctb {
namespace engine {

class Fist;

class Flag;

class Game;

enum class Team;

/// enum for possible animations of the player
enum class PlayerAnimation {
    ANIM_GO,
    ANIM_AIM,
    ANIM_SHOOT,
    ANIM_JUMP,

    ANIM_STAND,
    ANIM_JUMP_ATTACK,
    ANIM_HEAL,
    ANIM_TAUNT,
    //    ANIM_RUN,
    //    ANIM_DUCK,
};

/// enum to decide why and by which value an Player's score changes
enum class PlayerScoreFrom {
    SCORE_FLAG,        // increase score
    SCORE_PLAYER,      // increase score
    SCORE_PROJECTILE,  // increase score
    SCORE_DOOR,        // increase score
    SCORE_BOT,         // increase score
    SCORE_WON,         // increase score

    SCORE_RESPAWN  // reduce score
};

/// enum for posiible directions to move
enum class Direction { Left, Right };

/**
 * @brief 	The main player that interacts in the level
 */
class Player : public ActingRenderable {
   public:
    /**
     * @brief Constructor
     *
     * @param texture           which should be rendered
     * @param animationWidth    of the texture
     * @param animationHeight   of the texture
     * @param animationCount    of the texture
     */
    Player(Team team,
           SDL_Texture* texture,
           int animationWidth,
           int animationHeight,
           int animationCount);

    /**
     * @brief Destructor
     */
    ~Player() override;

    /**
     * @brief Render the texture of this object
     */
    void render() override;

    /**
     * @brief Render this object on a given position
     *
     * @param pos screen coordinate, on which this object should be rendered
     */
    virtual void renderStatic(Vector2dT& pos);

    /**
     * @brief Update the current position, changed by the new step of the b2World
     */
    void update() override;

    using PhysicalRenderable::addToWorld;

    /**
     * @brief Adds a physical representation of this player to the given b2World
     *
     * @param world to which the physical representation should be added
     * @param kinematics attributes of the physical representation
     */
    void addToWorld(b2World& world, ActingKinematics& kinematics) override;

    /**
     * @brief Perform a jump, if the player is on the ground
     */
    void jump() override;

    /**
     * @brief register input device
     *
     * @param input of the device
     */
    void registerInput(Input* input);

    /**
     * @brief deregister input
     */
    void deregisterInput();

    /***
     *  Handles Input from input devices
     *
     *  @param type
     *  @param input
     *  @param state
     *  @param angle
     */
    void handleInput(InputType type, Input* input, bool state, float angle);

    /***
     * Getter for the associated Input
     *
     * @return A Pointer to the mentioned Input
     */
    Input* getInput() { return m_input; }

    /***
     * Getter for the Player's score
     *
     * @return the positive score as uint64_t
     */
    uint64_t getScore() const { return m_score; }

    /**
     * @brief Alter the score of this player in regard to the parameter
     *
     * @param s value, that should be added to the score
     */
    void alterScore(PlayerScoreFrom from);

    /**
     * @brief signal, that this player has reached a door
     */
    void doorReached();

    /**
     * @brief Returns the players's health
     *
     * @return the health of the player
     */
    uint32_t getHealth() { return m_health; }

    /**
     * @brief Set the team of this player
     *
     * @param team to which this player should be added
     */
    void setTeam(Team team) { m_team = team; }

    /**
     * @brief Return the team of this player
     *
     * @return the team of this player
     */
    Team getTeam() { return m_team; }

    /**
     * @brief Return the weapon of this player
     *
     * @return the weapon of this player
     */
    Fist* getWeapon() const { return m_weapon; }

    /**
     * @brief Adds the given weapon to this player
     *
     * @param weapon which should be added to this player
     */
    void collectWeapon(Fist* weapon);

    /**
     * @brief Drops the current weapon of this player
     */
    void dropWeapon();

    /**
     * @brief Returns, if this player has the flag
     *
     * @return has this player the flag?
     */
    bool hasFlag() { return m_hasFlag; }

    /**
     * @brief Sets all attributes of this player to default
     */
    void reset() override;

    /**
     * @brief Sets, if this player has the flag
     *
     * @param b has this player the flag?
     */
    void setHasFlag(bool b);

    /**
     * @brief Sets the game, this player belongs to
     *
     * @param game to which this player belongs to
     */
    void setGame(Game* game) { m_game = game; }

    /**
     * @brief Adds the given damage to this player
     *
     * @param damage which should be added to this player
     * @param damageFrom from whom comes the damage?
     */
    void addDamage(uint32_t damage = 0, Player* damageFrom = nullptr);

    /**
     * @brief Restore the full health of this player
     */
    void setFullHealth() { m_health = 100; }

    /**
     * @brief Returns, if this player is alive
     *
     * @return is this player alive?
     */
    bool alive() { return m_health > 0; }

    /**
     * @brief Returns, if this player is in a healing process
     *
     * @return is this player in a healing process?
     */
    bool isHealing() { return SDL_GetTicks() < m_cooldownHeal; }

    /**
     * @brief Returns the next melee-tick
     *
     * @return the tick of the next melee
     */
    Uint32 nextMeleeTick() { return m_nextMeleeTick; }

    /**
     * @brief Resets the melee-tick, depending on the current tick
     *
     * @param currTick current tick
     */
    void resetMeleeTick(Uint32 currTick = 0) { m_nextMeleeTick = currTick + 500; }

    /**
     * @brief Returns, if the player is moving to the right
     *
     * @return Is the player moving to the right
     */
    inline bool isMovingRight() const { return m_right; }

    /**
     * @brief Returns, if the player is moving to the left
     *
     * @return Is the player moving to the left
     */
    inline bool isMovingLeft() const { return m_left; }

   private:
    /// Number of HP gain per heal
    static constexpr uint32_t kHpPerHeal = 25;

    /**
     * @brief heal the player's health
     */
    void heal();

    /**
     * @brief Get to the next animation of the texture
     */
    virtual void nextAnimation();

    /**
     * @brief Plays the step sound, if 1/5 of a second since the last call has passed.
     */
    virtual void playStepSound();

    /**
     * @brief Returns the time elapsed since the last call of this function
     *
     * @return the time elapsed since the last call of this function
     */
    float getElapsedTime();

    /**
     * @brief Choose an animation for the player, that depends on the input
     *
     * @param type of the input
     * @param state
     */
    void chooseAnimation(InputType type, bool state);

    /**
     * @brief change animation if player is not on ground
     */
    void handleJumpAnimations();

    /**
     * @brief handle running
     */
    void run();

    /// moving right
    bool m_right;

    /// moving left
    bool m_left;

    /// associated input device
    Input* m_input;

    uint64_t m_score;

    uint64_t m_inputHandlerId;

    /// maps possible states to a pair of (m_firstFrame, m_numFrames)
    std::map<PlayerAnimation, std::pair<int, int>> m_anim;

    /// current state to find correct sprites
    PlayerAnimation m_currAnimation;

    /// Is this player running
    bool m_running;

    /// Is this player shooting
    bool m_shooting;

    uint32_t m_health;

    /// The team
    Team m_team;

    /// Has this player the flag?
    bool m_hasFlag;

    /// Game in which this player exist
    Game* m_game;

    /// save last ticks for step sounds
    Fist* m_weapon;

    Uint32 m_lastSoundTicks;

    /// Movingdirection of the player
    Direction m_direction;

    /// Cooldown for thw healing method
    Uint32 m_cooldownHeal;

    /// Was there a healing animation
    bool m_healingAnimationDone;

    /// Flag for the update method. Should the weapon be dropped?
    bool m_dropWeaponOnUpdate{false};

    Uint32 m_nextMeleeTick;

    /// Angle of the weapon
    float m_angle;
};

}  // namespace engine
}  // namespace ctb

#endif  // ENGINE_SCENE_PLAYER_HPP
