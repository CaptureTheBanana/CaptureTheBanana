// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef COMMON_UTILS_HPP
#define COMMON_UTILS_HPP

#include <string>

#include <boost/filesystem.hpp>

namespace ctb {
namespace common {
namespace utils {

// Convert wchar to char on Windows
static constexpr char OS_SEPARATOR =
    static_cast<char>(boost::filesystem::path::preferred_separator);

/// Unix path seperator - internal used by CTB and config files
static constexpr char CTB_SEPARATOR = '/';

/// \brief Normalize the given path.
///
/// \param path
/// \return std::string Returns normalized path
inline std::string canonical(const std::string& path) {
    return boost::filesystem::canonical(path).string();
}

/// \brief Returns the filename with extension.
///
/// \param path Full path
/// \return std::string Filename with extension
inline std::string getFileName(const std::string& path) {
    boost::filesystem::path p(path);
    return p.filename().string();
}

/// \brief Returns the direcotry path.
///        e.g. /tmp/a.txt --> /tmp
///             /tmp/a     --> /tmp
///             /tmp/a/    --> /tmp/a
///
/// \param path Full path
/// \return std::string Parent direcotry path
inline std::string getDirectoryPath(const std::string& path) {
    boost::filesystem::path p(path);
    return p.parent_path().string();
}

/// \brief Returns the direcotry path with ending slash.
///        e.g. /tmp/a.txt --> /tmp/
///             /tmp/a     --> /tmp/
///             /tmp/a/    --> /tmp/a/
///
/// \param path Full path
/// \return std::string Parent direcotry path
inline std::string getDirectoryPathWithSlash(const std::string& path) {
    std::string dir = getDirectoryPath(path);
    if (dir.empty()) {
        return "";
    }
    return dir + OS_SEPARATOR;
}

/// \brief Return the path to the users home directory.
///
/// \note Returns documents directory on windows
///
/// \return std::string Directory path
std::string getHomePath();

/// \brief Windows compatibility function to converts backslashes to slashes.
///
/// \note On UNIX no conversion occurs!
///
/// \param path Unix/Windows path
/// \return std::wstring Unix path
inline std::string toUnixPath(const std::string& path) {
    boost::filesystem::path p(path);
    return p.generic_string();
}

/// \brief Converts the path to the prefered OS format.
///
/// \note On UNIX no conversion occurs!
///
/// \param path Unix/Windows path
/// \return std::wstring Windows path
inline std::string toOsPath(const std::string& path) {
    boost::filesystem::path p(path);
    return p.make_preferred().string();
}

#ifdef _WIN32

/// \brief Converts a UTF8 string to a unicode string.
///
/// \param str UTF8 string
/// \return std::wstring Unicode string
std::wstring fromUtf8(const std::string& str);

/// \brief Converts a unicode string to a UTF8 string.
///
/// \param str Unicode string
/// \return std::string UTF8 string
std::string toUtf8(const std::wstring& str);

#endif

}  // namespace utils
}  // namespace common
}  // namespace ctb

#endif  // COMMON_UTILS_HPP
