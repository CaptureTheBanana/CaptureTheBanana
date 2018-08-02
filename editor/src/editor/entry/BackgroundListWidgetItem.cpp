// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "editor/entry/BackgroundListWidgetItem.hpp"

namespace ctb {
namespace editor {

BackgroundListWidgetItem::BackgroundListWidgetItem(const QString& filename, int speed) {
    m_filename = filename.toStdString();
    m_scrollSpeed = speed;
}

}  // namespace editor
}  // namespace ctb
