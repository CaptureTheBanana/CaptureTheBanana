// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef _DOOR_HPP
#define _DOOR_HPP

#include "engine/graphics/PhysicalRenderable.hpp"

namespace ctb {
namespace engine {

// forward declaration
enum class Team;

/// Class representing a door the teams have to reach
class Door : public PhysicalRenderable {
   public:
    /**
     * @brief constructs a door
     *
     * @param team              which team has to reach this door
     * @param texture           the texture of the door
     * @param animationWidth    the width of each animation
     * @param animationHeight   the height of each animation
     * @param animationCount    the animation count
     * @param offset            the offset to render the door with, if it's bigger than one tile
     */
    Door(Team team,
         SDL_Texture* texture,
         int animationWidth,
         int animationHeight,
         int animationCount,
         int offset);

    /// adds the door to the given world
    virtual void addToThisWorld(b2World& world);

    /// return the team which has to reach this door
    Team getTeam() const { return m_team; }

    /// sets the team which has to reach this door
    void setTeam(const Team t) { m_team = t; }

    /// overriding to take offset into account
    void setPosition(const Vector2dT& v) override;

    /// Destructor
    ~Door() override = default;

   private:
    /// which team has to reach this door?
    Team m_team;

    /// position offset
    int m_offset;
};

}  // namespace engine
}  // namespace ctb

#endif
