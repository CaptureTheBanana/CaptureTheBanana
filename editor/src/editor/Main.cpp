// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <QApplication>
#include <QDesktopWidget>

#include <boost/core/demangle.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <common/ConsoleColor.hpp>
#include <common/Exceptions.hpp>

#include "editor/gui/MainWindow.hpp"

// Leak sanitizer suppression list
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
extern "C" {
const char* __lsan_default_suppressions() {
    return "leak:libdbus-1.so\n"
           "leak:libfontconfig.so\n"
           "leak:libglib-2.0.so\n"
           "leak:libxcb.so\n";
}
}
#endif
#endif

int main(int argc, char* argv[]) {
    using namespace ctb::common;
    using MainWindowT = ctb::editor::MainWindow;

    QApplication a(argc, argv);
    try {
        MainWindowT w;
        // Center the window on the screen
        w.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, w.size(),
                                          qApp->desktop()->availableGeometry()));
        w.show();
        return a.exec();
    } catch (const ctb::UserExceptionBaseT& ex) {
        std::cout << console::red << "[ERROR] [" << boost::core::demangle(typeid(ex).name()) << "] "
                  << ex.what() << console::reset << std::endl;
    } catch (const boost::property_tree::xml_parser_error& ex) {
        std::cout << console::red << "[ERROR] [xml_parser_error] " << ex.what() << console::reset
                  << std::endl;
    }
    return 1;
}
