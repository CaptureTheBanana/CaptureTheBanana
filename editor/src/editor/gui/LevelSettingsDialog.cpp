// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <QMessageBox>

#include "editor/gui/LevelSettingsDialog.hpp"

namespace ctb {
namespace editor {

LevelSettingsDialog::LevelSettingsDialog(QWidget* parent)
    : QDialog(parent), m_ui(std::make_unique<Ui::LevelSettingsDialog>()) {
    m_ui->setupUi(this);

    m_ui->cbType->addItem("Center");
    m_ui->cbType->addItem("Default");
    m_ui->cbType->addItem("End");

    connect(m_ui->btnHelp, SIGNAL(clicked()), this, SLOT(btnHelpClicked()));
}

LevelSettingsDialog::LevelTypeT LevelSettingsDialog::getLevelType() {
    switch (m_ui->cbType->currentIndex()) {
        case 0:
            return LevelTypeT::CENTER;
        case 2:
            return LevelTypeT::END;
        case 1:
        default:
            return LevelTypeT::DEFAULT;
    }
}

void LevelSettingsDialog::setLevelType(LevelSettingsDialog::LevelTypeT type) {
    switch (type) {
        case LevelTypeT::CENTER:
            m_ui->cbType->setCurrentIndex(0);
            break;
        case LevelTypeT::END:
            m_ui->cbType->setCurrentIndex(2);
            break;
        case LevelTypeT::DEFAULT:
        default:
            m_ui->cbType->setCurrentIndex(1);
            break;
    }
}

void LevelSettingsDialog::btnHelpClicked() {
    QMessageBox::information(
        this, "Help", tr("<b>Center:</b> Level is always in the middle of a game and schould be "
                         "symmetrical.<br><b>Default:</b> Levels are in between center and end "
                         "levels.<br><b>End:</b> Levels are at the end of the world."),
        QMessageBox::Ok);
}

}  // namespace editor
}  // namespace ctb
