// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef GRAPHICS_TILE_ITEM_HPP
#define GRAPHICS_TILE_ITEM_HPP

#include <QAction>
#include <QColor>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPixmap>
#include <QRect>
#include <memory>
#include <unordered_map>
#include <utility>  // std::pair

#include <gsl/gsl>
#include <parser/DynamicTilestore.hpp>

#include "editor/SpawnInfo.hpp"

namespace ctb {
namespace editor {

/// Tileset item callback action
enum class TilesetItemCallbackAction {
    KNone = 0,
    kDetails = 1,
    kRemoveTile = 2,
    kMarkTile = 3,
    kSetSpawn = 4
};

/// Information about the tile position
struct TilePosition {
    /// Width of the tile
    int width{0};
    // Height of the tile
    int height{0};
    /// Item offset
    QPoint itemOffset{0, 0};
};

/// Class that draws a tile of the layer/tileset
class GraphicsTilesetItem final : public QGraphicsItem {
   public:
    using CoordinateT = std::pair<int /*x*/, int /*y*/>;
    using CallbackT = std::function<void(TilesetItemCallbackAction, CoordinateT)>;

    using DynamicTilestorePtrT = gsl::not_null<ctb::DynamicTilestore*>;
    using TileIdToPixmapMapT = std::unordered_map<int /*tile id*/, QPixmap>;
    using TileIdToPixmapMapPtrT = gsl::not_null<TileIdToPixmapMapT*>;
    using SpawnInfoPtrT = gsl::not_null<SpawnInfoT*>;

   public:
    /// \brief Draws a tile from the layer/tileset at given position.
    ///
    /// \param tilePosition Tile position
    /// \param tilestore Pointer to the tilestore - must stay alive until the class is
    ///                  finished/killed.
    /// \param spawnInfo Pointer to the spawn information - must stay alive until the class is
    ///                  finished/killed.
    /// \param tileset Tile id to QPixmap map - must stay alive until the class is finished/killed.
    /// \param coordinate Grid coordinate
    /// \param callback Contextmenu action callback
    GraphicsTilesetItem(TilePosition tilePosition,
                        DynamicTilestorePtrT tilestore,
                        SpawnInfoPtrT spawnInfo,
                        TileIdToPixmapMapPtrT tilesetMap,
                        CoordinateT coordinate,
                        CallbackT&& callback);

    ~GraphicsTilesetItem() override = default;

    /// \brief Defines the outer bounds of the item as a rectangle.
    ///
    /// \return QRectF Rectangle
    QRectF boundingRect() const override { return m_boundingBox; }

    /// \brief Provide the item's painting.
    ///
    /// \param painter Painter
    /// \param option Style options
    /// \param widget Points to the widget that is being painted on or null.
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    /// Mark this tile
    void mark();

   protected:
    /// \brief Is called when the mouse is pressed on the item.
    ///
    /// \param event Mouse information
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    /// \brief Is called when the right mouse button is pressed on the item.
    ///
    /// \param event Mouse information
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

   private:
    void details();
    void removeTile();
    void removeMark();
    void setMark();
    void setSpawn();

    /// Tile information (width, height and offset)
    TilePosition m_tileInfo;
    /// Coordinate tile id mapping - non-owning
    DynamicTilestorePtrT m_tilestore;
    /// Spawn information  - non-owning
    SpawnInfoPtrT m_spawnInfo;
    /// Tile id bitmap mapping - non-owning
    TileIdToPixmapMapPtrT m_tilesetMap;

    /// The item coordinate
    CoordinateT m_coordinate;
    /// Contextmenu action callback
    CallbackT m_callback;

    /// Bounding box
    QRectF m_boundingBox;

    // Contexmenu stuff
    std::unique_ptr<QMenu> m_contextMenu{nullptr};
    std::unique_ptr<QAction> m_detailsAct{nullptr};
    std::unique_ptr<QAction> m_setMarkAct{nullptr};
    std::unique_ptr<QAction> m_removeMarkAct{nullptr};
    std::unique_ptr<QAction> m_removeAct{nullptr};
};

/// Class that draws a tile of the tilesheet
class GraphicsTileSelectorItem final : public QGraphicsItem {
   public:
    using CoordinateT = std::pair<int /*x*/, int /*y*/>;
    using CallbackT = std::function<void(int /*tile id*/)>;

    using QPixmapPtrT = gsl::not_null<QPixmap*>;

   public:
    /// \brief Draws a tile from the tilesheet at given position.
    ///
    /// \param tilePosition Tile information
    /// \param pixmap Pointer to tileset bitmap - must stay alive until the class is
    ///               finished/killed.
    /// \param tileId Tile id
    /// \param callback Callback action
    GraphicsTileSelectorItem(TilePosition tilePosition,
                             QPixmapPtrT pixmap,
                             int tileId,
                             CallbackT&& callback);

    ~GraphicsTileSelectorItem() override = default;

    /// \brief Defines the outer bounds of the item as a rectangle.
    ///
    /// \return QRectF Rectangle
    QRectF boundingRect() const override { return m_boundingBox; }

    /// \brief Provide the item's painting.
    ///
    /// \param painter Painter
    /// \param option Style options
    /// \param widget Points to the widget that is being painted on or null.
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

   protected:
    /// \brief Is called when the mouse is pressed on the item.
    ///
    /// \param event Mouse information
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

   private:
    /// Tile information (width, height and offset)
    TilePosition m_tileInfo;
    /// Pointer to the bitmap
    QPixmapPtrT m_pixmap;
    /// Tile index
    int m_tileId;
    /// Callback action
    CallbackT m_callback;

    /// bounding box
    QRectF m_boundingBox;
};

}  // namespace editor
}  // namespace ctb

#endif
