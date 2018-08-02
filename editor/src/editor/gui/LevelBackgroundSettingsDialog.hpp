// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef LEVELBACKGROUNDSETTINGSDIALOG_HPP
#define LEVELBACKGROUNDSETTINGSDIALOG_HPP

#include <QDialog>
#include <memory>
#include <string>
#include <vector>

#include <gsl/gsl>
#include <parser/BackgroundConfig.hpp>

#include "editor/ui_LevelBackgroundSettingsDialog.h"

namespace ctb {
namespace editor {

class LevelBackgroundSettingsDialog final : public QDialog {
    Q_OBJECT

   public:
    using BackgroundConfigVectorT = std::vector<parser::BackgroundConfig>;

   public:
    /// \brief Constructor
    ///
    /// \param backgrounds Background list
    /// \param absoluteGamePath Normalized absolute game directory path
    /// \param parent Parent widget or null
    explicit LevelBackgroundSettingsDialog(const BackgroundConfigVectorT& backgrounds,
                                           const std::string& absoluteGamePath,
                                           QWidget* parent = nullptr);
    ~LevelBackgroundSettingsDialog() override = default;

    /// Returns the background list.
    const BackgroundConfigVectorT& getList() { return m_backgroundList; }

   private slots:
    /// Button add clicked.
    void on_btnAdd_clicked();
    /// Button edit clicked.
    void on_btnEdit_clicked();
    /// Button remove clicked.
    void on_btnRemove_clicked();

    /// Button cancel clicked.
    void on_btnCancel_clicked();
    /// Button ok clicked.
    void on_btnOk_clicked();

   private:
    // Qt UI
    std::unique_ptr<Ui::LevelBackgroundSettingsDialog> m_ui;

    /// Absolute directory path to the game.xml
    std::string m_absoluteGamePath;

    /// Background list
    BackgroundConfigVectorT m_backgroundList{};
};

}  // namespace editor
}  // namespace ctb

#endif  // LEVELBACKGROUNDSETTINGSDIALOG_HPP
