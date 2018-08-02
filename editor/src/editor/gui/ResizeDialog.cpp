// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "editor/gui/ResizeDialog.hpp"

namespace ctb {
namespace editor {

ResizeDialog::ResizeDialog(QWidget* parent)
    : QDialog(parent), m_ui(std::make_unique<Ui::ResizeDialog>()) {
    m_ui->setupUi(this);
    m_ui->sbHeight->setMinimum(1);
    m_ui->sbHeight->setMaximum(9999);
    m_ui->sbWidth->setMinimum(1);
    m_ui->sbWidth->setMaximum(9999);

    connect(m_ui->sbHeight, SIGNAL(valueChanged(int)), this, SLOT(sbHeightValueChanged(int)));
    connect(m_ui->sbWidth, SIGNAL(valueChanged(int)), this, SLOT(sbWidthValueChanged(int)));
}

void ResizeDialog::sbHeightValueChanged(int value) {
    m_height = value;
}

void ResizeDialog::sbWidthValueChanged(int value) {
    m_width = value;
}

}  // namespace editor
}  // namespace ctb
