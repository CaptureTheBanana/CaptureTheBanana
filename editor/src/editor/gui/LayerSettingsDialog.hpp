// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef LAYERSETTINGSDIALOG_HPP
#define LAYERSETTINGSDIALOG_HPP

#include <QDialog>
#include <memory>

#include "editor/ui_LayerSettingsDialog.h"

namespace ctb {
namespace editor {

class LayerSettingsDialog final : public QDialog {
    Q_OBJECT

   public:
    explicit LayerSettingsDialog(QWidget* parent = nullptr);
    ~LayerSettingsDialog() override = default;

    /// Returns layer position
    int getLayerPosition() { return m_ui->sbLayer->value(); }

    /// \brief Sets layer position
    ///
    /// \param index Position
    void setLayerPosition(int index) { m_ui->sbLayer->setValue(index); }

    /// Gets friction value
    float getFriction() { return static_cast<float>(m_ui->sbFriction->value()); }

    /// \brief Sets friction value
    ///
    /// \param friction Friction
    void setFriction(float friction) { m_ui->sbFriction->setValue(friction); }

    /// Returns whether or not tiles of this layer collide with bots.
    bool canCollideBots() { return m_ui->cbCollideBots->isChecked(); }

    /// \brief Sets whether bots collide with tiles of this layer
    ///
    /// \param collide true if bots collide with tiles of this layer
    void setCollideBots(bool collide) { m_ui->cbCollideBots->setChecked(collide); }

    /// Returns whether or not tiles of this layer collide with projectiles
    bool canCollideProjectiles() { return m_ui->cbCollideProjectiles->isChecked(); }

    /// \brief Sets whether projectiles collide with tiles of this layer
    ///
    /// \param collide true if projectiles collide with tiles of this layer
    void setCollideProjectiles(bool collide) { m_ui->cbCollideProjectiles->setChecked(collide); }

    /// Returns whether or not tiles of this layer collide with players
    bool canCollidePlayers() { return m_ui->cbCollidePlayers->isChecked(); }

    /// \brief Sets whether players collide with tiles of this layer
    ///
    /// \param collide true if players collide with tiles of this layer
    void setCollidePlayers(bool collide) { m_ui->cbCollidePlayers->setChecked(collide); }

   private:
    std::unique_ptr<Ui::LayerSettingsDialog> m_ui;
};

}  // namespace editor
}  // namespace ctb

#endif  // LAYERSETTINGSDIALOG_HPP
