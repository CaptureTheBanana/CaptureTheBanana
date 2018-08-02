// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <stdexcept>
#ifdef _WIN32
// Don't move this include; otherwise compilation will fail
#include <initguid.h>

#include <Knownfolders.h>
#include <ShlObj.h>
#include <Windows.h>
#else
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <gsl/gsl>

#include "common/Utils.hpp"

namespace ctb {
namespace common {
namespace utils {

std::string getHomePath() {
#ifdef _WIN32
    PWSTR path = nullptr;
    auto defer = [&] {
        if (path != nullptr) {
            CoTaskMemFree(path);
        }
    };
    if (SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &path) != S_OK) {
        std::runtime_error("WIN32: SHGetKnownFolderPath failed!");
    }
    Ensures(path != nullptr);
    return toUtf8(std::wstring(path));
#else
    // NOTE: Do not free 'homePath' or 'pwd'!
    const char* homePath = std::getenv("HOME");
    if (homePath != nullptr) {
        return homePath;
    }

    struct passwd* pwd = getpwuid(getuid());
    if (pwd == nullptr) {
        std::runtime_error("getpwuid failed!");
    }
    std::string path = pwd->pw_dir;  // NOLINT
    return path;
#endif
}

#ifdef _WIN32

std::wstring fromUtf8(const std::string& str) {
    if (str.empty()) {
        return {};
    }

    int bufSize =
        MultiByteToWideChar(CP_UTF8, 0, &str[0], static_cast<int>(str.size()), nullptr, 0);
    std::wstring buf(bufSize, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], static_cast<int>(str.size()), &buf[0], bufSize);
    return buf;
}

std::string toUtf8(const std::wstring& str) {
    if (str.empty()) {
        return {};
    }

    int bufSize = WideCharToMultiByte(CP_UTF8, 0, &str[0], static_cast<int>(str.size()), nullptr, 0,
                                      nullptr, nullptr);
    std::string buf(bufSize, 0);
    WideCharToMultiByte(CP_UTF8, 0, &str[0], static_cast<int>(str.size()), &buf[0], bufSize,
                        nullptr, nullptr);
    return buf;
}

#endif
}  // namespace utils
}  // namespace common
}  // namespace ctb
