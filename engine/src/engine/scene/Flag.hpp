/*
 *  Flag.hpp
 *
 *      Author: Joshua Sangmeister
 */

#ifndef ENGINE_SCENE_FLAG_HPP
#define ENGINE_SCENE_FLAG_HPP

#include "engine/graphics/PhysicalRenderable.hpp"

namespace ctb {
namespace engine {

class Player;

/// Class representing the Fla- eeh, banana.
class Flag : public PhysicalRenderable {
   public:
    /**
     * @brief Constructor
     *
     * @param texture           pointer to the texture
     * @param animationWidth    width of each animation in the texture
     * @param animationHeight   height of each animation in the texture
     * @param animationCount    number of animations in the texture
     */
    Flag(SDL_Texture* texture, int animationWidth, int animationHeight, int animationCount);

    /// add the flag to world
    void addToThisWorld(b2World& world);

    /// If true, the flag is bound to a player.
    bool isInUse() { return m_isInUse; }

    /// Set if the flag is bound to an actor
    void setInUse(bool isInUse) { m_isInUse = isInUse; }

    /// shorthand for setInUse(false)
    void setNotInUse() { m_isInUse = false; }

    /// Destructor
    ~Flag() override = default;

   private:
    /// is this flag in use?
    bool m_isInUse{false};
};

}  // namespace engine
}  // namespace ctb

#endif
