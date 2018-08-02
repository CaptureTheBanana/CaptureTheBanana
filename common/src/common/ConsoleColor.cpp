// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "common/ConsoleColor.hpp"

#if __unix__ || __APPLE__
#define IS_UNIX 1
#define IS_WINDOWS 0
#elif _WIN32
#define IS_UNIX 0
#define IS_WINDOWS 1
#else
#error Unknown OS!
#endif

#if IS_WINDOWS
#ifndef NOMINMAX
#define NOMINMAX
#endif  // NOMINMAX
#include <windows.h>
#endif

namespace ctb {
namespace common {
namespace console {
namespace {
#if IS_WINDOWS

/// Retruns the console handle.
HANDLE getConsoleHandle(std::ostream& stream, bool& isOutputHandle) {
    if (&std::cout == &stream) {
        isOutputHandle = true;
        return GetStdHandle(STD_OUTPUT_HANDLE);
    } else if (&std::cerr == &stream) {
        isOutputHandle = false;
        return GetStdHandle(STD_ERROR_HANDLE);
    }
    return INVALID_HANDLE_VALUE;
}

/// Returns current console attributes.
WORD getConsoleAttributes(HANDLE console) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(console, &info)) {
        return 0;
    }
    return info.wAttributes;
}

#endif
}  // namespace

std::ostream& setColor(std::ostream& stream, ConsoleColor color) {
#if IS_WINDOWS
    static WORD outputHandleAttributes = 0;
    static WORD errorHandleAttributes = 0;

    bool isOutputHandle = true;
    HANDLE console = getConsoleHandle(stream, isOutputHandle);
    if (console == INVALID_HANDLE_VALUE) {
        return stream;
    }

    // Save default attributes
    if (isOutputHandle && outputHandleAttributes == 0) {
        outputHandleAttributes = getConsoleAttributes(console);
    } else if (!isOutputHandle && errorHandleAttributes == 0) {
        errorHandleAttributes = getConsoleAttributes(console);
    }

    WORD attributes = getConsoleAttributes(console);
    if (attributes == 0) {
        return stream;
    }

    // Remove foreground color
    attributes &= ~(attributes & 0x0F);

    switch (color) {
        case ConsoleColor::kReset:
            attributes = isOutputHandle ? outputHandleAttributes : errorHandleAttributes;
            break;
        case ConsoleColor::kBlack:
            attributes |= FOREGROUND_INTENSITY;  // gray (black: 0)
            break;
        case ConsoleColor::kGreen:
            attributes |= FOREGROUND_GREEN;
            break;
        case ConsoleColor::kRed:
            attributes |= FOREGROUND_RED;
            break;
        case ConsoleColor::kWhite:
            attributes |=
                FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        case ConsoleColor::kYellow:
            attributes |= FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;
            break;
        default:
            return stream;
    }

    SetConsoleTextAttribute(console, attributes);
    return stream;
#else
    switch (color) {
        case ConsoleColor::kReset:
            stream << "\033[0m";
            break;
        case ConsoleColor::kBlack:
            stream << "\033[30m";
            break;
        case ConsoleColor::kGreen:
            stream << "\033[32m";
            break;
        case ConsoleColor::kRed:
            stream << "\033[31m";
            break;
        case ConsoleColor::kWhite:
            stream << "\033[37m";
            break;
        case ConsoleColor::kYellow:
            stream << "\033[33m";
            break;
        default:
            break;
    }
    return stream;
#endif
}

}  // namespace console
}  // namespace common
}  // namespace ctb
