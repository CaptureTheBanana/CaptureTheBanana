// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef COMMON_CONSOLECOLOR_HPP
#define COMMON_CONSOLECOLOR_HPP

#include <iostream>

namespace ctb {
namespace common {
namespace console {

/// Foreground and background colors for the console.
enum class ConsoleColor {
    kReset = 0,

    // Foreground colors
    kBlack = 30,
    kRed = 31,
    kGreen = 32,
    kYellow = 33,
    kWhite = 37
};

/// \brief Sets the console color to the given color.
///
/// \param stream Output stream
/// \param color Console color
/// \return std::ostream Output stream
std::ostream& setColor(std::ostream& stream, ConsoleColor color);

/// \brief Sets the console color to the given color.
///
/// \param stream Output stream
/// \param color Console color
/// \return std::ostream &operator Output stream
inline std::ostream& operator<<(std::ostream& stream, const ConsoleColor& color) {
    return setColor(stream, color);
}

/// \brief Sets the console color to black.
///
/// \param stream Output stream
/// \return std::ostream Output stream
inline std::ostream& black(std::ostream& stream) {
    return setColor(stream, ConsoleColor::kBlack);
}

/// \brief Sets the console color to green.
///
/// \param stream Output stream
/// \return std::ostream Output stream
inline std::ostream& green(std::ostream& stream) {
    return setColor(stream, ConsoleColor::kGreen);
}

/// \brief Sets the console color to red.
///
/// \param stream Output stream
/// \return std::ostream Output stream
inline std::ostream& red(std::ostream& stream) {
    return setColor(stream, ConsoleColor::kRed);
}

/// \brief Resets the console color.
///
/// \param stream Output stream
/// \return std::ostream Output stream
inline std::ostream& reset(std::ostream& stream) {
    return setColor(stream, ConsoleColor::kReset);
}

/// \brief Sets the console color to white.
///
/// \param stream Output stream
/// \return std::ostream Output stream
inline std::ostream& white(std::ostream& stream) {
    return setColor(stream, ConsoleColor::kWhite);
}

/// \brief Sets the console color to yellow.
///
/// \param stream Output stream
/// \return std::ostream Output stream
inline std::ostream& yellow(std::ostream& stream) {
    return setColor(stream, ConsoleColor::kYellow);
}

}  // namespace console
}  // namespace common

using ConsoleColorT = common::console::ConsoleColor;

}  // namespace ctb

#endif  // COMMON_CONSOLECOLOR_HPP
