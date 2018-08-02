// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "editor/gui/LayerSettingsDialog.hpp"

namespace ctb {
namespace editor {

LayerSettingsDialog::LayerSettingsDialog(QWidget* parent)
    : QDialog(parent), m_ui(std::make_unique<Ui::LayerSettingsDialog>()) {
    m_ui->setupUi(this);
}

}  // namespace editor
}  // namespace ctb
