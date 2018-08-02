// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <QString>
#include <cmath>
#include <fstream>

#include "editor/scene/GraphicsItems.hpp"
#include "editor/scene/GraphicsScenes.hpp"

namespace ctb {
namespace editor {

TilesetScene::TilesetScene(LevelConfigPtrT level,
                           DynamicTilestorePtrT tilestore,
                           SpawnInfoPtrT spawnInfo,
                           TileIdToPixmapMapPtrT tilesetMap,
                           GraphicsTilesetItem::CallbackT&& callback)
    : m_tilesetMap(tilesetMap), m_callback(std::forward<GraphicsTilesetItem::CallbackT>(callback)) {
    // Set scene size
    setSceneRect(0.0, 0.0, level->getWidth() * level->getTileWidth(),
                 level->getHeight() * level->getTileHeight());

    // Initialize items/tiles
    for (int y = 0; y < level->getHeight(); ++y) {
        for (int x = 0; x < level->getWidth(); ++x) {
            CoordinateT coordinate = std::make_pair(x, y);
            TilePosition position{level->getTileWidth(),
                                  level->getTileHeight(),
                                  {x * level->getTileWidth(), y * level->getTileHeight()}};

            auto* item = new GraphicsTilesetItem(
                position, tilestore, spawnInfo, tilesetMap, coordinate,
                [&](TilesetItemCallbackAction a, CoordinateT c) { m_callback(a, c); });

            // 'addItem' takes ownership
            addItem(item);
        }
    }
}

void TilesetScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    // This forwards the event to the item.
    QGraphicsScene::mousePressEvent(event);
}

void TilesetScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    QPointF mousePosition = event->scenePos();
    QTransform transform;
    auto* item =
        dynamic_cast<GraphicsTilesetItem*>(itemAt(mousePosition.x(), mousePosition.y(), transform));
    if (item != nullptr) {
        item->mark();
    }
    // This forwards the event to the item.
    QGraphicsScene::mouseMoveEvent(event);
}

// ----------------------------------------------------------------------------

TileSelectorScene::TileSelectorScene(LevelConfigPtrT level,
                                     TileIdToPixmapMapPtrT tilesetMap,
                                     GraphicsTileSelectorItem::CallbackT&& callback)
    : m_tilesetMap(tilesetMap),
      m_callback(std::forward<GraphicsTileSelectorItem::CallbackT>(callback)) {
    const int maxTileId = level->getMaxTileId();
    Expects(maxTileId >= 0);

    // Calculate scene size
    int tilesPerRow = static_cast<int>(std::floor(400.0 / (level->getTileWidth() + kItemOffset)));

    // Tile height with padding
    const int tileHeight = level->getTileHeight() + kItemOffset;

    int tilePerColumn = static_cast<int>(
        std::ceil(static_cast<double>(maxTileId) / static_cast<double>(tilesPerRow)));

    // Scene size + tile height (empty tile)
    double height = (tilePerColumn * tileHeight + tilesPerRow * tileHeight) + tileHeight;

    // Set scene size
    setSceneRect(0.0, 0.0, 400.0, height);

    // Tile id 0 is a empty texture and 1 to "maxTileId" are tiles from the tilesheet.
    int tileId = 0;
    for (int y = 0; tileId <= maxTileId; ++y) {
        for (int x = 0; x < tilesPerRow && tileId <= maxTileId; ++x) {
            // Set offset between tiles
            int offX = x * (level->getTileWidth() + kItemOffset);
            int offY = y * (level->getTileHeight() + kItemOffset);
            if (x == 0) {
                offX += 2;
            } else if (x == tilesPerRow - 1) {
                offX -= 2;
            }

            TilePosition position{level->getTileWidth(), level->getTileHeight(), {offX, offY}};
            auto* item = new GraphicsTileSelectorItem(position, &m_tilesetMap->at(tileId), tileId,
                                                      [&](int id) { m_callback(id); });

            // 'addItem' takes ownership
            addItem(item);
            ++tileId;
        }
    }
}

void TileSelectorScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    // This forwards the event to the item.
    QGraphicsScene::mousePressEvent(event);
}

}  // namespace editor
}  // namespace ctb
