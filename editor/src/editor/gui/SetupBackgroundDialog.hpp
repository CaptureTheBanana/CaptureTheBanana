// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef SETUPBACKGROUNDDIALOG_HPP
#define SETUPBACKGROUNDDIALOG_HPP

#include <QDialog>
#include <QListWidget>
#include <memory>
#include <string>

#include "editor/entry/BackgroundListWidgetItem.hpp"
#include "editor/ui_SetupBackgroundDialog.h"

namespace ctb {
namespace editor {

class SetupBackgroundDialog : public QDialog {
    Q_OBJECT

   public:
    explicit SetupBackgroundDialog(QWidget* parent = nullptr);

    ~SetupBackgroundDialog() override = default;

    /// Set the UI elements
    void setUi(const std::string& filename, int scrollSpeed);

    /// Current item or null
    std::unique_ptr<BackgroundListWidgetItem> m_item{nullptr};

   private slots:
    void on_btnCancel_clicked();

    void on_btnFileDialog_clicked();

    void on_btnSave_clicked();

    void on_tbPath_textChanged(const QString& arg);

   private:
    std::unique_ptr<Ui::SetupBackgroundDialog> m_ui;

    /// \brief Returns the filename with extension.
    ///
    /// \param path Full path
    /// \return QString Filename with extension
    QString getFileName(const QString& path);
};

}  // namespace editor
}  // namespace ctb

#endif  // SETUPBACKGROUNDDIALOG_HPP
