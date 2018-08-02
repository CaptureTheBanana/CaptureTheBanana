// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_PHYSICS_PHYSICALTILESET_HPP
#define ENGINE_PHYSICS_PHYSICALTILESET_HPP

#include "engine/graphics/TilesetRenderable.hpp"
#include "engine/physics/PhysicalObject.hpp"

namespace ctb {
namespace engine {

/**
 * @brief Class, that represents a TileSet with a physical representation for every Tile in it
 */
class PhysicalTileSet : public PhysicalObject, public TilesetRenderable {
   public:
    /**
     * @brief Constructor
     *
     * @param layc TileSet configuration data
     * @param levc Level configuration data
     * @param flipped Should the TileSet be vertical reflected
     */
    PhysicalTileSet(parser::TilesetConfig& layc, parser::LevelConfig* levc, bool flipped);

    /**
     * @brief adds a physical representation for every tile and ground shapes to the world
     *
     * @param world to which the physical representations should be added
     * @param kinematics attributes of the physical representations
     */
    void addToWorld(b2World& world, Kinematics& kinematics) override;

   private:
    /**
     * @brief Adds a new edge to the given world
     *
     * @param start of the edge
     * @param end  of the edge
     * @param world to which the edge should be added
     * @param kinematics attributes of the edge
     * @param height height of the edge
     */
    void createNewEdge(float32 start,
                       float32 end,
                       b2World& world,
                       Kinematics& kinematics,
                       int height);
};

}  // namespace engine
}  // namespace ctb
#endif
