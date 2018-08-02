// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <QFileDialog>
#include <QMessageBox>

#include <boost/filesystem.hpp>
#include <common/Utils.hpp>

#include "editor/gui/SetupBackgroundDialog.hpp"

namespace ctb {
namespace editor {

SetupBackgroundDialog::SetupBackgroundDialog(QWidget* parent)
    : QDialog(parent), m_ui(std::make_unique<Ui::SetupBackgroundDialog>()) {
    m_ui->setupUi(this);
}

void SetupBackgroundDialog::setUi(const std::string& filename, int scrollSpeed) {
    m_ui->tbPath->setText(QString::fromStdString(filename));
    m_ui->sbScrollSpeed->setValue(scrollSpeed);
}

void SetupBackgroundDialog::on_btnCancel_clicked() {
    this->close();
}

void SetupBackgroundDialog::on_btnFileDialog_clicked() {
    QString filename = QFileDialog::getOpenFileName(this, "Open tilesheet", ".", "Image (*.png)");
    if (!filename.isEmpty()) {
        m_ui->tbPath->setText(filename);
    }
}

void SetupBackgroundDialog::on_btnSave_clicked() {
    QPalette emptyPalette;
    emptyPalette.setColor(QPalette::Base, Qt::red);

    if (m_ui->tbPath->text().isEmpty()) {
        m_ui->tbPath->setPalette(emptyPalette);

        QMessageBox::information(this, "", "The input is incomplete.", QMessageBox::Ok);
        return;
    }

    m_item = std::make_unique<BackgroundListWidgetItem>(m_ui->tbPath->text(),
                                                        m_ui->sbScrollSpeed->value());
    m_item->setText(getFileName(m_ui->tbPath->text()));
    this->done(QDialog::Accepted);
}

void SetupBackgroundDialog::on_tbPath_textChanged(const QString& arg) {
    QPalette neutralPalette;
    neutralPalette.setColor(QPalette::Base, Qt::white);
    m_ui->tbPath->setPalette(neutralPalette);

    QFileInfo fileInfo(arg);
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        m_ui->tbPath->setText("");
    }
}

QString SetupBackgroundDialog::getFileName(const QString& path) {
    return QString::fromStdString(common::utils::getFileName(path.toStdString()));
}

}  // namespace editor
}  // namespace ctb
