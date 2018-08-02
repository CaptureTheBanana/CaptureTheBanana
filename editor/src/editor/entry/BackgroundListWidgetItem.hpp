// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef BACKGROUNDLISTQIDGETITEM_HPP
#define BACKGROUNDLISTQIDGETITEM_HPP

#include <QListWidgetItem>
#include <QObject>

namespace ctb {
namespace editor {

class BackgroundListWidgetItem : public QListWidgetItem {
   public:
    explicit BackgroundListWidgetItem(const QString& filename, int speed);

    ~BackgroundListWidgetItem() override = default;

    /// Returns the filename
    virtual std::string getFilename() { return m_filename; }

    /// \brief Sets the filename
    ///
    /// \param filename Filename
    virtual void setFilename(std::string filename) { m_filename = filename; }

    /// Returns scroll speed
    virtual int getScrollSpeed() { return m_scrollSpeed; }

    /// \brief Sets the scroll speed
    ///
    /// \param filename Scroll speed
    virtual void setScrollSpeed(int scrollSpeed) { m_scrollSpeed = scrollSpeed; }

   private:
    std::string m_filename;
    int m_scrollSpeed;
};

}  // namespace editor
}  // namespace ctb

#endif  // BACKGROUNDLISTQIDGETITEM_HPP
