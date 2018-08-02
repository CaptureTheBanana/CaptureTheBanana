// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef SETUPDIALOG_HPP
#define SETUPDIALOG_HPP

#include <QDialog>
#include <QFileDialog>
#include <memory>
#include <string>

#include <parser/GameParser.hpp>

#include "editor/entry/BackgroundListWidgetItem.hpp"

namespace Ui {
class SetupDialog;
}

namespace ctb {
namespace editor {

class SetupDialog : public QDialog {
    Q_OBJECT

   public:
    explicit SetupDialog(QWidget* parent = nullptr);

    ~SetupDialog() override;

    /// \brief Sets the game textbox visibility.
    ///
    /// \param visible Visible
    void setGameTextBoxVisibility(bool visible = true);

    /// Level configuration
    std::unique_ptr<ctb::parser::LevelConfig> m_level{nullptr};
    /// Path to the game.xml
    std::string m_gameConfigPath;

   private slots:
    void on_btnCancel_clicked();

    void on_btnContinue_clicked();

    void on_btnSelectTilesheet_clicked();

    void on_btnSelectGameXml_clicked();

    void on_btnBack_clicked();

    void on_btnFinish_clicked();

    void on_btnAdd_clicked();

    void on_btnRemove_clicked();

    void on_tbLevelName_textChanged(const QString& arg);

    void on_tbLevelWidth_textChanged(const QString& arg);

    void on_tbLevelHeight_textChanged(const QString& arg);

    void on_tbTilesheetPath_textChanged(const QString& arg);

    void on_tbTileWidth_textChanged(const QString& arg);

    void on_tbTileHeight_textChanged(const QString& arg);

    void on_tbTilesPerRow_textChanged(const QString& arg);

    void on_tbTileOffset_textChanged(const QString& arg);

    void on_tbMaxTileId_textChanged(const QString& arg);

    void on_tbGamePath_textChanged(const QString& arg);

   private:
    Ui::SetupDialog* m_ui;
    bool m_isGameTextboxVisible{false};

    QPalette m_neutralPalette;
};

}  // namespace editor
}  // namespace ctb

#endif  // SETUPDIALOG_HPP
