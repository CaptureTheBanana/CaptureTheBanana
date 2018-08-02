// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <parser/BotConfig.hpp>
#include <parser/WeaponConfig.hpp>

#include "editor/gui/TileDialog.hpp"

namespace ctb {
namespace editor {

TileDialog::TileDialog(QWidget* parent)
    : QDialog(parent), m_ui(std::make_unique<Ui::TileDialog>()) {
    m_ui->setupUi(this);

    m_ui->cbBotSpawn->addItem("None");
    m_ui->cbWeaponSpawn->addItem("None");
}

void TileDialog::addBots(std::vector<TileDialog::BotConfigT*>& bots) {
    for (auto* bot : bots) {
        m_ui->cbBotSpawn->addItem(QString::fromStdString(bot->getName()));
    }
}

void TileDialog::addWeapons(std::vector<TileDialog::WeaponConfigT*>& weapons) {
    for (auto* weapon : weapons) {
        m_ui->cbWeaponSpawn->addItem(QString::fromStdString(weapon->getName()));
    }
}

}  // namespace editor
}  // namespace ctb
