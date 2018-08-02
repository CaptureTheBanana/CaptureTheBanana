// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <QMessageBox>
#include <QString>
#include <QTranslator>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <common/Utils.hpp>
#include <gsl/gsl>

#include "editor/gui/SetupBackgroundDialog.hpp"
#include "editor/gui/SetupDialog.hpp"
#include "editor/ui_SetupDialog.h"

namespace ctb {
namespace editor {
namespace {
namespace cutils = ctb::common::utils;

/// \brief Returns a part of \param str.
///
/// \param str Input string
/// \param remove String to remove
/// \return std::string
std::string substr(std::string str, const std::string& remove) {
    str.erase(str.find(remove), remove.size());
    return str;
}

/// \brief Validate the given game.xml
///
/// \param path game.xml path
/// \return bool Returns true if valid
bool validateGameXml(const std::string& path) {
    using boost::property_tree::ptree;
    try {
        ptree pt;
        read_xml(path, pt);
        pt.get_child("game");
        return true;
    } catch (...) {
        return false;
    }
}
}  // namespace

SetupDialog::SetupDialog(QWidget* parent)
    : QDialog(parent), m_level(nullptr), m_ui(new Ui::SetupDialog) {
    m_ui->setupUi(this);
    m_ui->stackedWidget->setCurrentIndex(0);
    m_ui->lwBackgrounds->setDragDropMode(QAbstractItemView::InternalMove);

    m_ui->tbLevelWidth->setValidator(new QIntValidator(0, 9999, this));
    m_ui->tbLevelHeight->setValidator(new QIntValidator(0, 9999, this));
    m_ui->tbTileWidth->setValidator(new QIntValidator(0, 999, this));
    m_ui->tbTileHeight->setValidator(new QIntValidator(0, 999, this));
    m_ui->tbTilesPerRow->setValidator(new QIntValidator(0, 9999, this));
    m_ui->tbTileOffset->setValidator(new QIntValidator(0, 9999, this));
    m_ui->tbMaxTileId->setValidator(new QIntValidator(0, 9999, this));

    m_neutralPalette.setColor(QPalette::Base, Qt::white);
}

SetupDialog::~SetupDialog() {
    delete m_ui;
}

void SetupDialog::setGameTextBoxVisibility(bool visible) {
    m_isGameTextboxVisible = visible;
    m_ui->tbGamePath->setVisible(visible);
    m_ui->btnSelectGameXml->setVisible(visible);
    m_ui->label_13->setVisible(visible);
}

void SetupDialog::on_btnCancel_clicked() {
    this->close();
}

void SetupDialog::on_btnContinue_clicked() {
    bool incomplete = false;
    QPalette emptyPalette;
    emptyPalette.setColor(QPalette::Base, Qt::red);

    if (m_ui->tbLevelName->text().isEmpty()) {
        m_ui->tbLevelName->setPalette(emptyPalette);
        incomplete = true;
    }
    if (m_ui->tbLevelWidth->text().isEmpty()) {
        m_ui->tbLevelWidth->setPalette(emptyPalette);
        incomplete = true;
    }
    if (m_ui->tbLevelHeight->text().isEmpty()) {
        m_ui->tbLevelHeight->setPalette(emptyPalette);
        incomplete = true;
    }
    if (m_ui->tbTilesheetPath->text().isEmpty()) {
        m_ui->tbTilesheetPath->setPalette(emptyPalette);
        incomplete = true;
    }
    if (m_ui->tbTileWidth->text().isEmpty()) {
        m_ui->tbTileWidth->setPalette(emptyPalette);
        incomplete = true;
    }
    if (m_ui->tbTileHeight->text().isEmpty()) {
        m_ui->tbTileHeight->setPalette(emptyPalette);
        incomplete = true;
    }
    if (m_ui->tbTilesPerRow->text().isEmpty()) {
        m_ui->tbTilesPerRow->setPalette(emptyPalette);
        incomplete = true;
    }
    if (m_ui->tbTileOffset->text().isEmpty()) {
        m_ui->tbTileOffset->setPalette(emptyPalette);
        incomplete = true;
    }
    if (m_ui->tbMaxTileId->text().isEmpty()) {
        m_ui->tbMaxTileId->setPalette(emptyPalette);
        incomplete = true;
    }
    if (m_isGameTextboxVisible && m_ui->tbGamePath->text().isEmpty()) {
        m_ui->tbGamePath->setPalette(emptyPalette);
        incomplete = true;
    }

    if (incomplete) {
        QMessageBox::information(this, "", "The input is incomplete.", QMessageBox::Ok);
        return;
    } else if (!boost::filesystem::portable_name(m_ui->tbLevelName->text().toStdString())) {
        QMessageBox::information(this, "Level name", "No special characters or spaces allowed.",
                                 QMessageBox::Ok);
        return;
    } else if (m_isGameTextboxVisible && !validateGameXml(m_ui->tbGamePath->text().toStdString())) {
        QMessageBox::information(this, "", "Invalid game.xml file.", QMessageBox::Ok);
        return;
    }
    m_ui->stackedWidget->setCurrentIndex(1);
}

void SetupDialog::on_btnSelectTilesheet_clicked() {
    QString filename = QFileDialog::getOpenFileName(this, "Open tilesheet", ".", "Image (*.png)");
    if (!filename.isEmpty()) {
        m_ui->tbTilesheetPath->setText(filename);
    }
}

void SetupDialog::on_btnSelectGameXml_clicked() {
    QString filename =
        QFileDialog::getOpenFileName(this, "Open game file", ".", "Game file (game.xml)");
    if (!filename.isEmpty()) {
        m_ui->tbGamePath->setText(filename);
    }
}

void SetupDialog::on_btnBack_clicked() {
    m_ui->stackedWidget->setCurrentIndex(0);
}

void SetupDialog::on_btnFinish_clicked() {
    if (m_isGameTextboxVisible) {
        m_gameConfigPath = m_ui->tbGamePath->text().toStdString();
    }

    // All resource files must be in the same directory as the game.xml or in a subdirectory.
    QFileInfo gameInfo(QString::fromStdString(m_gameConfigPath));
    QFileInfo tilesheetInfo(m_ui->tbTilesheetPath->text());
    std::string normalizedGamePath = gameInfo.canonicalPath().toStdString() + cutils::CTB_SEPARATOR;

    if (!tilesheetInfo.canonicalPath().startsWith(gameInfo.canonicalPath())) {
        QMessageBox::critical(this, "Error",
                              "All resource files must be in the same directory as the game.xml "
                              "or in a subdirectory.",
                              QMessageBox::Ok);
        return;
    }

    std::vector<ctb::parser::BackgroundConfig> buf{};
    for (int i = 0; i < m_ui->lwBackgrounds->count(); i++) {
        auto* item = dynamic_cast<BackgroundListWidgetItem*>(m_ui->lwBackgrounds->item(i));
        Expects(item != nullptr);

        QFileInfo bgInfo(QString::fromStdString(item->getFilename()));
        if (!bgInfo.canonicalPath().startsWith(gameInfo.canonicalPath())) {
            QMessageBox::critical(this, "Error",
                                  "All resource files must be in the same directory as the "
                                  "game.xml or in a subdirectory.",
                                  QMessageBox::Ok);
            return;
        }

        buf.emplace_back(i, item->getScrollSpeed(),
                         substr(bgInfo.canonicalFilePath().toStdString(), normalizedGamePath),
                         normalizedGamePath);
    }

    m_level = std::make_unique<ctb::parser::LevelConfig>();

    // Add background layers to LevelConfig
    for (auto& bg : buf) {
        m_level->addBackground(bg);
    }

    m_level->setTilesheetFile(
        substr(m_ui->tbTilesheetPath->text().toStdString(), normalizedGamePath));
    m_level->setTileWidth(m_ui->tbTileWidth->text().toInt());
    m_level->setTileHeight(m_ui->tbTileHeight->text().toInt());
    m_level->setTileOffset(m_ui->tbTileOffset->text().toInt());
    m_level->setTilesPerRow(m_ui->tbTilesPerRow->text().toInt());
    m_level->setMaxTileId(m_ui->tbMaxTileId->text().toInt());
    m_level->setWidth(m_ui->tbLevelWidth->text().toInt());
    m_level->setHeight(m_ui->tbLevelHeight->text().toInt());
    m_level->setLevelType(ctb::parser::LevelType::DEFAULT);
    m_level->setGravitation(20.0f);

    std::string levelFilename = m_ui->tbLevelName->text().toStdString();
    if (!m_ui->tbLevelName->text().endsWith(".xml")) {
        levelFilename += ".xml";
    }
    m_level->setLevelFilename(levelFilename);
    m_level->setAbsolutePath(normalizedGamePath);

    done(QDialog::Accepted);
}

void SetupDialog::on_btnAdd_clicked() {
    SetupBackgroundDialog backgroundDialog(this);
    backgroundDialog.setWindowTitle("Background configuration");
    backgroundDialog.setModal(true);
    int result = backgroundDialog.exec();
    if (result == QDialog::Accepted && backgroundDialog.m_item != nullptr) {
        // 'addItem' takes ownership
        m_ui->lwBackgrounds->addItem(backgroundDialog.m_item.release());
    }
}

void SetupDialog::on_btnRemove_clicked() {
    for (QListWidgetItem* item : m_ui->lwBackgrounds->selectedItems()) {
        delete m_ui->lwBackgrounds->takeItem((m_ui->lwBackgrounds->row(item)));
    }
}

void SetupDialog::on_tbLevelName_textChanged(const QString& /*arg*/) {
    m_ui->tbLevelName->setPalette(m_neutralPalette);
}

void SetupDialog::on_tbLevelWidth_textChanged(const QString& /*arg*/) {
    m_ui->tbLevelWidth->setPalette(m_neutralPalette);
}

void SetupDialog::on_tbLevelHeight_textChanged(const QString& /*arg*/) {
    m_ui->tbLevelHeight->setPalette(m_neutralPalette);
}

void SetupDialog::on_tbTilesheetPath_textChanged(const QString& arg) {
    m_ui->tbTilesheetPath->setPalette(m_neutralPalette);

    QFileInfo checkFile(arg);
    if (!checkFile.exists() || !checkFile.isFile()) {
        m_ui->tbTilesheetPath->setText("");
    }
}

void SetupDialog::on_tbTileWidth_textChanged(const QString& /*arg*/) {
    m_ui->tbTileWidth->setPalette(m_neutralPalette);
}

void SetupDialog::on_tbTileHeight_textChanged(const QString& /*arg*/) {
    m_ui->tbTileHeight->setPalette(m_neutralPalette);
}

void SetupDialog::on_tbTilesPerRow_textChanged(const QString& /*arg*/) {
    m_ui->tbTilesPerRow->setPalette(m_neutralPalette);
}

void SetupDialog::on_tbTileOffset_textChanged(const QString& /*arg*/) {
    m_ui->tbTileOffset->setPalette(m_neutralPalette);
}

void SetupDialog::on_tbMaxTileId_textChanged(const QString& /*arg*/) {
    m_ui->tbMaxTileId->setPalette(m_neutralPalette);
}

void SetupDialog::on_tbGamePath_textChanged(const QString& arg) {
    m_ui->tbGamePath->setPalette(m_neutralPalette);

    QFileInfo fileInfo(arg);
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        m_ui->tbGamePath->setText("");
    }
}

}  // namespace editor
}  // namespace ctb
