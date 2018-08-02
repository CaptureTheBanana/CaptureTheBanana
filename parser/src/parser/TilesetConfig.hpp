// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef PARSER_TILESETCONFIG_HPP
#define PARSER_TILESETCONFIG_HPP

#include <string>

namespace ctb {
namespace parser {

/// \brief describes a layer of tiles
class TilesetConfig {
   public:
    /// \param layer determines when this layer gets rendered
    /// \param friciton friction of players walking on tiles of this layer
    /// \param collideBots if set, bots collide with tiles of this layer
    /// \param collideProjectiles if set, projectiles collide with tiles of this layer
    /// \param collidePlayers if set, players collide with tiles of this layer
    /// \param tileArrangement arrangement of Tile ids: 2 3 12 4 2 34
    TilesetConfig(int layer,
                  float friction,
                  bool collideBots,
                  bool collideProjectiles,
                  bool collidePlayers,
                  std::string tileArrangement)
        : m_layer(layer),
          m_friction(friction),
          m_collideBots(collideBots),
          m_collideProjectiles(collideProjectiles),
          m_collidePlayers(collidePlayers),
          m_tileArrangement(std::move(tileArrangement)) {}

    /// \brief sets layer that determines when this layer gets rendered
    ///
    /// \param layer new layer
    inline void setLayer(int layer) { m_layer = layer; }

    /// \brief delivers layer of this layer
    ///
    /// \return int layer
    inline int getLayer() { return m_layer; }

    /// \brief sets the friction of players movving on tiles
    ///
    /// \param friction new friction
    inline void setFriction(float friction) { m_friction = friction; }

    /// \brief delivers friction for players walking on tiles of this layer
    ///
    /// \return float friction of players
    inline float getFriction() { return m_friction; }

    /// \brief sets whether or not bots collide with tiles of this layer
    ///
    /// \param collideBots true if bots collide with tiles of this layer
    inline void setCollideBots(bool collideBots) { m_collideBots = collideBots; }

    /// \brief delivers whether or not tiles of this layer collide with bots
    ///
    /// \return bool true if bots collide with tiles of this layer
    inline bool canCollideBots() { return m_collideBots; }

    /// \brief sets whether or not projectiles collide with tiles of this layer
    ///
    /// \param collideProjectiles true if projectiles collide with tiles of this layer
    inline void setCollideProjectiles(bool collideProjectiles) {
        m_collideProjectiles = collideProjectiles;
    }

    /// \brief delivers whether or not tiles of this layer collide with projectiles
    ///
    /// \return bool true if projectiles collide with tiles of this layer
    inline bool canCollideProjectiles() { return m_collideProjectiles; }

    /// \brief sets whether or not players collide with tiles of this layer
    ///
    /// \param collidePlayers true if players collide with tiles of this layer
    inline void setCollidePlayers(bool collidePlayers) { m_collidePlayers = collidePlayers; }

    /// \brief delivers whether or not tiles of this layer collide with players
    ///
    /// \return bool true if players collide with tiles of this layer
    inline bool canCollidePlayers() { return m_collidePlayers; }

    /// \brief sets tile arrangement
    ///         the format looks like this: 2 3 12 4 2 34
    ///
    /// \param tileArrangement new arrangement of tile ids
    inline void setTileArrangement(std::string tileArrangement) {
        m_tileArrangement = tileArrangement;
    }

    /// \brief delivers arrangement of tile ids
    ///         the format looks like this: 2 3 12 4 2 34
    ///
    /// \return std::string arrangement of tiles
    inline std::string getTileArrangement() { return m_tileArrangement; }

    virtual ~TilesetConfig() = default;

   private:
    /// defines time when layer gets rendered
    int m_layer;
    /// friction of tiles if players move on them
    float m_friction;
    /// do bots collide with blocks of this layer?
    bool m_collideBots;
    /// do Projectiles collide with blocks of this layer?
    bool m_collideProjectiles;
    /// do Players collide with blocks of this layer?
    bool m_collidePlayers;
    /// tile-ids to arrange the tiles on screen seperated by ','
    std::string m_tileArrangement;
};

}  // namespace parser
}  // namespace ctb

#endif
