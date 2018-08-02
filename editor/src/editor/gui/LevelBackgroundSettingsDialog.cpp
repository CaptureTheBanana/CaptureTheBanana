// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <QFileInfo>
#include <QMessageBox>

#include <common/Utils.hpp>

#include "editor/entry/BackgroundListWidgetItem.hpp"
#include "editor/gui/LevelBackgroundSettingsDialog.hpp"
#include "editor/gui/SetupBackgroundDialog.hpp"

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
    auto it = str.find(remove);
    if (it == std::string::npos) {
        throw std::runtime_error("Invalid string");
    }
    str.erase(it, remove.size());
    return str;
}
}  // namespace

LevelBackgroundSettingsDialog::LevelBackgroundSettingsDialog(
    const BackgroundConfigVectorT& backgrounds,
    const std::string& absoluteGamePath,
    QWidget* parent)
    : QDialog(parent),
      m_ui(std::make_unique<Ui::LevelBackgroundSettingsDialog>()),
      m_absoluteGamePath(cutils::canonical(absoluteGamePath)) {
    m_ui->setupUi(this);
    m_ui->lwBackgrounds->setDragDropMode(QAbstractItemView::InternalMove);

    // Fill ui list
    for (auto& background : backgrounds) {
        auto item = std::make_unique<BackgroundListWidgetItem>(
            QString::fromStdString(background.getImageFilename()), background.getScrollSpeed());
        item->setText(
            QString::fromStdString(common::utils::getFileName(background.getImageFilename())));
        // 'addItem' takes ownership
        m_ui->lwBackgrounds->addItem(item.release());
    }
}

void LevelBackgroundSettingsDialog::on_btnAdd_clicked() {
    SetupBackgroundDialog dialog(this);
    dialog.setWindowTitle("Background configuration");
    dialog.setModal(true);
    int result = dialog.exec();
    if (result == QDialog::Accepted && dialog.m_item != nullptr) {
        // 'addItem' takes ownership
        m_ui->lwBackgrounds->addItem(dialog.m_item.release());
    }
}

void LevelBackgroundSettingsDialog::on_btnEdit_clicked() {
    if (m_ui->lwBackgrounds->selectedItems().empty()) {
        return;
    }

    auto* item =
        dynamic_cast<BackgroundListWidgetItem*>(m_ui->lwBackgrounds->selectedItems().at(0));
    Expects(item != nullptr);

    SetupBackgroundDialog dialog(this);
    dialog.setWindowTitle("Background configuration");
    dialog.setModal(true);
    dialog.setUi(item->getFilename(), item->getScrollSpeed());
    int result = dialog.exec();
    if (result == QDialog::Accepted && dialog.m_item != nullptr) {
        // Update item and ui
        item->setFilename(dialog.m_item->getFilename());
        item->setScrollSpeed(dialog.m_item->getScrollSpeed());
        item->setText(QString::fromStdString(cutils::getFileName(item->getFilename())));
        m_ui->lwBackgrounds->update();
    }
}

void LevelBackgroundSettingsDialog::on_btnRemove_clicked() {
    for (QListWidgetItem* item : m_ui->lwBackgrounds->selectedItems()) {
        delete m_ui->lwBackgrounds->takeItem((m_ui->lwBackgrounds->row(item)));
    }
}

void LevelBackgroundSettingsDialog::on_btnCancel_clicked() {
    close();
}

void LevelBackgroundSettingsDialog::on_btnOk_clicked() {
    m_backgroundList.clear();
    for (int i = 0; i < m_ui->lwBackgrounds->count(); i++) {
        auto* item = dynamic_cast<BackgroundListWidgetItem*>(m_ui->lwBackgrounds->item(i));
        Expects(item != nullptr);

        QFileInfo bgInfo(QString::fromStdString(item->getFilename()));
        if (!bgInfo.canonicalPath().startsWith(QString::fromStdString(m_absoluteGamePath))) {
            QMessageBox::critical(this, "Error",
                                  "All resource files must be in the same directory as the "
                                  "game.xml or in a subdirectory.",
                                  QMessageBox::Ok);
            m_backgroundList.clear();
            return;
        }

        m_backgroundList.emplace_back(
            i, item->getScrollSpeed(),
            substr(bgInfo.canonicalFilePath().toStdString(), m_absoluteGamePath),
            m_absoluteGamePath);
    }
    done(QDialog::Accepted);
}

}  // namespace editor
}  // namespace ctb
