// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef RESIZEDIALOG_HPP
#define RESIZEDIALOG_HPP

#include <QDialog>
#include <memory>

#include <gsl/gsl>

#include "editor/ui_ResizeDialog.h"

namespace ctb {
namespace editor {

/// Resize dialog
class ResizeDialog final : public QDialog {
    Q_OBJECT

   public:
    explicit ResizeDialog(QWidget* parent = nullptr);
    ~ResizeDialog() override = default;

    /// Returns height
    int getHeight() { return m_height; }

    /// \brief Sets height
    /// \pre height >= 1
    ///
    /// \param height Height
    void setHeight(int height) {
        Expects(height >= 1);
        m_height = height;
        m_ui->sbHeight->setValue(m_height);
    }

    /// Returns width
    int getWidth() { return m_width; }

    /// \brief Sets width
    /// \pre width >= 1
    ///
    /// \param width Width
    void setWidth(int width) {
        Expects(width >= 1);
        m_width = width;
        m_ui->sbWidth->setValue(m_width);
    }

   public slots:
    void sbHeightValueChanged(int value);
    void sbWidthValueChanged(int value);

   private:
    std::unique_ptr<Ui::ResizeDialog> m_ui;
    int m_height{1};
    int m_width{1};
};

}  // namespace editor
}  // namespace ctb

#endif  // RESIZEDIALOG_HPP
