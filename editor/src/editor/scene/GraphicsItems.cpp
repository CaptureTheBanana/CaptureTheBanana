// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <QObject>
#include <QPainter>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <utility>

#include "editor/gui/TileDialog.hpp"
#include "editor/scene/GraphicsItems.hpp"

namespace ctb {
namespace editor {

GraphicsTilesetItem::GraphicsTilesetItem(TilePosition tilePosition,
                                         DynamicTilestorePtrT tilestore,
                                         SpawnInfoPtrT spawnInfo,
                                         TileIdToPixmapMapPtrT tilesetMap,
                                         CoordinateT coordinate,
                                         CallbackT&& callback)
    : m_tileInfo(tilePosition),
      m_tilestore(tilestore),
      m_spawnInfo(spawnInfo),
      m_tilesetMap(tilesetMap),
      m_coordinate(std::move(coordinate)),
      m_callback(std::forward<CallbackT>(callback)),
      m_boundingBox(static_cast<qreal>(tilePosition.itemOffset.x()),
                    static_cast<qreal>(tilePosition.itemOffset.y()),
                    static_cast<qreal>(tilePosition.width),
                    static_cast<qreal>(tilePosition.height)) {
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);

    m_detailsAct = std::make_unique<QAction>("Details");
    m_setMarkAct = std::make_unique<QAction>("Set a mark");
    m_removeMarkAct = std::make_unique<QAction>("Remove a mark");
    m_removeAct = std::make_unique<QAction>("Remove");

    m_contextMenu = std::make_unique<QMenu>();
    m_contextMenu->addAction(m_detailsAct.get());
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(m_setMarkAct.get());
    m_contextMenu->addAction(m_removeMarkAct.get());
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(m_removeAct.get());
}

void GraphicsTilesetItem::paint(QPainter* painter,
                                const QStyleOptionGraphicsItem* /*option*/,
                                QWidget* /*widget*/) {
    int tileId = m_tilestore->get(m_coordinate.first, m_coordinate.second);
    const QPixmap& img = m_tilesetMap->at(tileId);

    // Draw bitmap
    QRectF srcRect{0.0, 0.0, static_cast<qreal>(m_tileInfo.width),
                   static_cast<qreal>(m_tileInfo.height)};
    painter->drawPixmap(m_boundingBox, img, srcRect);

    // Paint grid style
    QPen pen(QColor(240, 230, 140));  // Khaki (yellow)
    pen.setStyle(Qt::DashDotLine);
    pen.setWidth(1);
    if (tileId == 0) {
        pen.setWidth(2);
        pen.setColor(QColor(255, 255, 153));
    }
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    // Use brush to indicate spawn points.
    SpawnType spawnType = toSpawnType(m_spawnInfo->get(m_coordinate.first, m_coordinate.second));
    if (spawnType != SpawnType::kNone) {
        QBrush b(Qt::white);
        if (hasSpawnType(spawnType, SpawnType::kFlag)) {
            b.setStyle(Qt::DiagCrossPattern);
            setToolTip("Flag spawn");
        } else if (hasSpawnType(spawnType, SpawnType::kDoor)) {
            b.setStyle(Qt::CrossPattern);
            setToolTip("Door spawn");
        } else if (hasSpawnType(spawnType, SpawnType::kPlayer)) {
            b.setStyle(Qt::HorPattern);
            setToolTip("Player spawn");
        } else if (hasSpawnType(spawnType, SpawnType::kBot)) {
            b.setStyle(Qt::BDiagPattern);
            setToolTip("Bot spawn");
        } else if (hasSpawnType(spawnType, SpawnType::kWeapon)) {
            b.setStyle(Qt::FDiagPattern);
            setToolTip("Weapon spawn");
        } else {
            b.setStyle(Qt::NoBrush);
            setToolTip("");
        }
        painter->setBrush(b);
        painter->drawRect(m_boundingBox);

        if (hasSpawnType(spawnType, SpawnType::kMark)) {
            QBrush bMark;
            bMark.setColor(QColor("red"));
            bMark.setStyle(Qt::Dense6Pattern);
            painter->setBrush(bMark);
            painter->drawRect(m_boundingBox);
        }
    } else {
        setToolTip("");
    }
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(m_boundingBox);
}

void GraphicsTilesetItem::mark() {
    m_callback(TilesetItemCallbackAction::kMarkTile, m_coordinate);
}

void GraphicsTilesetItem::details() {
    m_callback(TilesetItemCallbackAction::kDetails, m_coordinate);
}

void GraphicsTilesetItem::removeTile() {
    m_callback(TilesetItemCallbackAction::kRemoveTile, m_coordinate);
}

void GraphicsTilesetItem::removeMark() {
    SpawnType type = toSpawnType(m_spawnInfo->get(m_coordinate.first, m_coordinate.second));
    removeSpawnType(type, SpawnType::kMark);
    m_spawnInfo->set(m_coordinate.first, m_coordinate.second, fromSpawnType(type));
    update();
}

void GraphicsTilesetItem::setMark() {
    // Set "mark" flag and update this item
    SpawnType type = toSpawnType(m_spawnInfo->get(m_coordinate.first, m_coordinate.second));
    setSpawnType(type, SpawnType::kMark);
    m_spawnInfo->set(m_coordinate.first, m_coordinate.second, fromSpawnType(type));
    update();
}

void GraphicsTilesetItem::setSpawn() {
    m_callback(TilesetItemCallbackAction::kSetSpawn, m_coordinate);
}

void GraphicsTilesetItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if (event->modifiers() == Qt::ControlModifier) {
            details();
        } else if (event->modifiers() == Qt::ShiftModifier) {
            removeTile();
        } else if (event->modifiers() == Qt::AltModifier) {
            setSpawn();
        } else {
            mark();
        }
    }
    QGraphicsItem::mousePressEvent(event);
}

void GraphicsTilesetItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event) {
    QAction* result = m_contextMenu->exec(event->screenPos());
    if (result == m_detailsAct.get()) {
        details();
    } else if (result == m_setMarkAct.get()) {
        setMark();
    } else if (result == m_removeMarkAct.get()) {
        removeMark();
    } else if (result == m_removeAct.get()) {
        removeTile();
    }
    QGraphicsItem::contextMenuEvent(event);
}

// ----------------------------------------------------------------------------

GraphicsTileSelectorItem::GraphicsTileSelectorItem(TilePosition tilePosition,
                                                   QPixmapPtrT pixmap,
                                                   int tileId,
                                                   CallbackT&& callback)
    : m_tileInfo(tilePosition),
      m_pixmap(pixmap),
      m_tileId(tileId),
      m_callback(std::forward<CallbackT>(callback)),
      m_boundingBox(static_cast<qreal>(tilePosition.itemOffset.x()),
                    static_cast<qreal>(tilePosition.itemOffset.y()),
                    static_cast<qreal>(tilePosition.width),
                    static_cast<qreal>(tilePosition.height)) {
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setCursor(QCursor(Qt::PointingHandCursor));
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void GraphicsTileSelectorItem::paint(QPainter* painter,
                                     const QStyleOptionGraphicsItem* /*option*/,
                                     QWidget* /*widget*/) {
    // Draw bitmap
    QRectF srcRect{0.0, 0.0, static_cast<qreal>(m_tileInfo.width),
                   static_cast<qreal>(m_tileInfo.height)};
    painter->drawPixmap(m_boundingBox, *m_pixmap, srcRect);

    // If selected draw border
    if (isSelected()) {
        QPen pen(QColor(0, 0, 0));
        pen.setStyle(Qt::DashDotLine);
        pen.setWidth(2);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
    } else {
        painter->setPen(Qt::NoPen);
    }
    painter->drawRect(m_boundingBox);
}

void GraphicsTileSelectorItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    m_callback(m_tileId);
    QGraphicsItem::mousePressEvent(event);
}

}  // namespace editor
}  // namespace ctb
