// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef LEVELSETTINGSDIALOG_HPP
#define LEVELSETTINGSDIALOG_HPP

#include <QDialog>
#include <memory>

#include <parser/LevelConfig.hpp>

#include "editor/ui_LevelSettingsDialog.h"

namespace ctb {
namespace editor {

class LevelSettingsDialog final : public QDialog {
    Q_OBJECT

   public:
    using LevelTypeT = ctb::parser::LevelType;

   public:
    /// \brief
    ///
    /// \param parent
    explicit LevelSettingsDialog(QWidget* parent = nullptr);
    ~LevelSettingsDialog() override = default;

    /// Returns the level type
    LevelTypeT getLevelType();

    /// \brief Sets the given level type.
    ///
    /// \param type Level type
    void setLevelType(LevelTypeT type);

    /// Returns gravitation
    float getGravitation() { return static_cast<float>(m_ui->sbGravitation->value()); }

    /// \brief Sets gravitation
    ///
    /// \param value Gravitation
    void setGravitation(float value) { m_ui->sbGravitation->setValue(value); }

   public slots:
    void btnHelpClicked();

   private:
    std::unique_ptr<Ui::LevelSettingsDialog> m_ui;
};

}  // namespace editor
}  // namespace ctb

#endif  // LEVELSETTINGSDIALOG_HPP
