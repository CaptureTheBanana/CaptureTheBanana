// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef TILEDIALOG_HPP
#define TILEDIALOG_HPP

#include <QDialog>
#include <QString>
#include <memory>
#include <string>
#include <vector>

#include "editor/ui_TileDialog.h"

namespace ctb {
namespace parser {
class BotConfig;
class WeaponConfig;
}  // namespace parser

namespace editor {

/// Dialog to select the tile spawn type.
class TileDialog final : public QDialog {
    Q_OBJECT

   public:
    using BotConfigT = ctb::parser::BotConfig;
    using WeaponConfigT = ctb::parser::WeaponConfig;

   public:
    explicit TileDialog(QWidget* parent = nullptr);
    ~TileDialog() override = default;

    /// \brief Adds the given list of bots to the bot context menu.
    ///
    /// \param bots Bot list
    void addBots(std::vector<BotConfigT*>& bots);

    /// \brief Adds the given list of weapons to the weapon context menu.
    ///
    /// \param weapons Weapon list
    void addWeapons(std::vector<WeaponConfigT*>& weapons);

    /// Returns true if left door is set.
    bool isDoorL() { return m_ui->rbDoorL->isChecked(); }

    /// \brief Sets the left door checkbox.
    ///
    /// \param isDoorL Is left door
    void setDoorL(bool isDoorL) { m_ui->rbDoorL->setChecked(isDoorL); }

    /// Returns true if right door is set.
    bool isDoorR() { return m_ui->rbDoorR->isChecked(); }

    /// \brief Sets the right door checkbox.
    ///
    /// \param isDoorR Is right door
    void setDoorR(bool isDoorR) { m_ui->rbDoorR->setChecked(isDoorR); }

    /// Returns true if flag spawn is set.
    bool isFlagSpawn() { return m_ui->rbFlagSpawn->isChecked(); }

    /// \brief Sets the flag checkbox.
    ///
    /// \param isSpawn Is flag spawn
    void setFlagSpawn(bool isSpawn) { m_ui->rbFlagSpawn->setChecked(isSpawn); }

    /// Returns true if player spawn is set.
    bool isPlayerSpawn() { return m_ui->rbPlayerSpawn->isChecked(); }

    /// \brief Sets the player checkbox.
    ///
    /// \param isSpawn Is player spawn
    void setPlayerSpawn(bool isSpawn) { m_ui->rbPlayerSpawn->setChecked(isSpawn); }

    /// Returns true if bot spawn is set.
    bool isBotSpawn() {
        return m_ui->rbBotSpawn->isChecked() && m_ui->cbBotSpawn->currentText() != "None";
    }

    /// \brief Sets the bot checkbox.
    ///
    /// \param isSpawn Is bot spawn
    void setBotSpawn(bool isSpawn) { m_ui->rbBotSpawn->setChecked(isSpawn); }

    /// Return true if weapon spawn is set.
    bool isWeaponSpawn() {
        return m_ui->rbWeaponSpawn->isChecked() && m_ui->cbWeaponSpawn->currentText() != "None";
    }

    /// \brief Sets the weapon checkbox.
    ///
    /// \param isSpawn Is weapon spawn
    void setWeaponSpawn(bool isSpawn) { m_ui->rbWeaponSpawn->setChecked(isSpawn); }

    /// Return the name of the selected box.
    std::string getBotName() { return m_ui->cbBotSpawn->currentText().toStdString(); }

    /// \brief Sets the name of the selected bot.
    ///
    /// \param name Bot name
    void setBotName(const std::string& name) {
        if (name.empty()) {
            m_ui->cbBotSpawn->setCurrentIndex(0);
        } else {
            m_ui->cbBotSpawn->setCurrentText(QString::fromStdString(name));
        }
    }

    /// Return the name of the selected weapon.
    std::string getWeaponName() { return m_ui->cbWeaponSpawn->currentText().toStdString(); }

    /// \brief Sets the name of the selected weapon.
    ///
    /// \param name Weapon name
    void setWeaponName(const std::string& name) {
        if (name.empty()) {
            m_ui->cbWeaponSpawn->setCurrentIndex(0);
        } else {
            m_ui->cbWeaponSpawn->setCurrentText(QString::fromStdString(name));
        }
    }

   private:
    std::unique_ptr<Ui::TileDialog> m_ui;
};

}  // namespace editor
}  // namespace ctb

#endif  // TILEDIALOG_HPP
