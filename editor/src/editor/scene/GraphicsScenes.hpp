// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef LEVEL_SCENE_HPP
#define LEVEL_SCENE_HPP

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>
#include <functional>  //std::hash
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>  //std::pair

#include <gsl/gsl>
#include <parser/DynamicTilestore.hpp>
#include <parser/LevelConfig.hpp>
#include <parser/TilesetConfig.hpp>

#include "editor/SpawnInfo.hpp"
#include "editor/scene/GraphicsItems.hpp"

class MainWindow;

namespace ctb {
namespace editor {

namespace details {
// NOTE: std::hash<std::pair<int, int>> in not well defined. So we need implement it ourself.
struct pair_hash {
    template <class T, class S>
    std::size_t operator()(const std::pair<T, S>& p) const {
        return std::hash<T>{}(p.first) ^ std::hash<S>{}(p.second);
    }
};
}  // namespace details

/// Layer/Tileset view
class TilesetScene final : public QGraphicsScene {
   public:
    using TilesetConfigT = ctb::parser::TilesetConfig;
    using TilesetConfigPtrT = gsl::not_null<TilesetConfigT*>;
    using LevelConfigT = ctb::parser::LevelConfig;
    using LevelConfigPtrT = gsl::not_null<LevelConfigT*>;

    using QPixmapPtrT = gsl::not_null<QPixmap*>;
    using TileIdToPixmapMapT = std::unordered_map<int /*tile set*/, QPixmap>;
    using TileIdToPixmapMapPtrT = gsl::not_null<TileIdToPixmapMapT*>;

    using CoordinateT = std::pair<int /*x*/, int /*y*/>;
    /// Maps coordinates to tile ids - eg. (0,0) --> 1
    using CoordinateTileMap = std::unordered_map<CoordinateT, int /*tile id*/, details::pair_hash>;

    using DynamicTilestorePtrT = gsl::not_null<ctb::DynamicTilestore*>;
    using SpawnInfoPtrT = gsl::not_null<SpawnInfoT*>;

   public:
    /// \brief Creates a layer/tileset view
    ///
    /// \param level Pointer to the level
    /// \param tilestore Pointer to the tilestore - must stay alive until the class is
    ///                  finished/killed.
    /// \param spawnInfo Pointer to the spawn information - must stay alive until the class is
    ///                  finished/killed.
    /// \param tileset Tile id to QPixmap map - must stay alive until the class is finished/killed.
    /// \param callback Contextmenu action callback
    TilesetScene(LevelConfigPtrT level,
                 DynamicTilestorePtrT tilestore,
                 SpawnInfoPtrT spawnInfo,
                 TileIdToPixmapMapPtrT tilesetMap,
                 GraphicsTilesetItem::CallbackT&& callback);

    ~TilesetScene() override = default;

   protected:
    /// \brief Is called whenever the mouse is pressend within the scene.
    ///
    /// \param event Mouse information
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

   private:
    /// Tile id to tile bitmap map
    TileIdToPixmapMapPtrT m_tilesetMap;
    /// Contextmenu action callback
    GraphicsTilesetItem::CallbackT m_callback;
};

/// Tilesheet view
class TileSelectorScene final : public QGraphicsScene {
   public:
    using TilesetConfigT = ctb::parser::TilesetConfig;
    using TilesetConfigPtrT = gsl::not_null<TilesetConfigT*>;
    using LevelConfigT = ctb::parser::LevelConfig;
    using LevelConfigPtrT = gsl::not_null<LevelConfigT*>;

    using QPixmapPtrT = gsl::not_null<QPixmap*>;
    using TileIdToPixmapMapT = std::unordered_map<int /*tile id*/, QPixmap>;
    using TileIdToPixmapMapPtrT = gsl::not_null<TileIdToPixmapMapT*>;

    using CoordinateT = std::pair<int /*x*/, int /*y*/>;
    /// Maps coordinates to tile ids - eg. (0,0) --> 1
    using CoordinateTileMap = std::unordered_map<CoordinateT, int /*tile id*/, details::pair_hash>;

    /// Offset between two items/tiles
    static constexpr int kItemOffset = 4;

   public:
    /// \brief Creates a new tilesheet view
    ///
    /// \param level Pointer to the level
    /// \param tileset Tile id to QPixmap map - must stay alive until the class is finished/killed.
    /// \param callback Callback
    TileSelectorScene(LevelConfigPtrT level,
                      TileIdToPixmapMapPtrT tilesetMap,
                      GraphicsTileSelectorItem::CallbackT&& callback);

    ~TileSelectorScene() override = default;

   protected:
    /// \brief Is called whenever the mouse is pressend within the scene.
    ///
    /// \param event Mouse information
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

   private:
    /// Tile id to tile bitmap map
    TileIdToPixmapMapPtrT m_tilesetMap;
    /// Callback action
    GraphicsTileSelectorItem::CallbackT m_callback;
};

}  // namespace editor
}  // namespace ctb

#endif
