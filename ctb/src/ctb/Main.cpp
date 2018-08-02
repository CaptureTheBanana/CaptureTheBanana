// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <string>
#include <typeinfo>
#include <vector>
#ifdef _WIN32
#include <Windows.h>
#endif

#include <CtbVersion.hpp>
#include <boost/core/demangle.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <clara.hpp>
#include <common/ConsoleColor.hpp>
#include <common/Exceptions.hpp>
#include <common/Utils.hpp>
#include <engine/Window.hpp>
#include <engine/util/SdlDriver.hpp>

using namespace ctb;
using namespace ctb::common;

// Leak sanitizer suppression list
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
extern "C" {
const char* __lsan_default_suppressions() {
    return "leak:libX11\n"
           "leak:__interceptor_malloc\n";
}
}
#endif
#endif

namespace cli {

using CliConfig = engine::WindowArguments;

enum class Status { kOk, kClose, kError };

/// Returns the application version
std::string version() {
#if defined(CTB_VERSION_MAJOR) && defined(CTB_VERSION_MINOR) && defined(CTB_VERSION_PATCH)
    std::string version = "Version: ";
    version.append(CTB_VERSION_MAJOR);
    version.push_back('.');
    version.append(CTB_VERSION_MINOR);
    version.push_back('.');
    version.append(CTB_VERSION_PATCH);
#ifdef CTB_VERSION_NAME
    version.append(CTB_VERSION_NAME);
#endif
    return version;
#else
    return "Version: n.a.";
#endif
}

/// \brief Parses commandline arguments
///
/// \param argc Argument count
/// \param argv Arguments
/// \param config Output configuration
/// \return Status Status code
Status parse(int argc, char** argv, CliConfig& config) {
    bool showHelp = false;
    bool showVersion = false;
    bool noSound = false;
    auto cli = clara::Help(showHelp) |
               clara::Opt(config.debug)["-d"]["--debug"]("enable debug mode") |
               clara::Opt(noSound)["-s"]["--no-sound"]("disable sound") |
               clara::Opt(showVersion)["-v"]["--version"]("show version information") |
               clara::Opt(config.verbose)["--verbose"]("show more debug information") |
               clara::Arg(config.path, "path")("path to the game.xml file");
    auto result = cli.parse(clara::Args(argc, argv));
    if (!result) {
        std::cerr << console::red << "Error in command line: " << result.errorMessage()
                  << console::reset << std::endl;
        return Status::kError;
    }

    config.sound = !noSound;

    if (showHelp) {
        std::cout << version() << "\n\n" << cli << std::endl;
        return Status::kClose;
    } else if (showVersion) {
        std::cout << version() << std::endl;
        return Status::kClose;
    }
    return Status::kOk;
}

/// \brief Main loop
///
/// \param argc Argument count
/// \param argv Arguments
/// \return int Exit code
int run(int argc, char** argv) {
    cli::CliConfig config{};
    cli::Status cliStatus = cli::Status::kError;
    if ((cliStatus = parse(argc, argv, config)) != cli::Status::kOk) {
        return cliStatus == cli::Status::kError ? 1 : 0;
    }

    if (config.path.empty()) {
        std::vector<std::string> paths = {"../res/game.xml", "../../res/game.xml"};
#ifdef _WIN32
        paths.push_back(utils::getHomePath() + "/CaptureTheBanana/res/game.xml");
#else
        paths.push_back(utils::getHomePath() + "/.CaptureTheBanana/res/game.xml");
        paths.emplace_back("/usr/share/capture-the-banana/res/game.xml");
#endif
        for (auto& item : paths) {
            if (boost::filesystem::exists(item)) {
                config.path = item;
                break;
            }
        }
    } else {
        if (boost::filesystem::is_directory(config.path)) {
            char c = config.path[config.path.size() - 1];
            if (c != '/' && c != '\\') {
                config.path.push_back('/');
            }
            config.path.append("game.xml");
        }

#ifdef _WIN32
        config.path = utils::toUnixPath(config.path);
#endif
    }

    if (config.path.empty()) {
        std::cout << console::red << "[ERROR] No gamefile found." << console::reset << std::endl;
        return 1;
    } else if (!boost::filesystem::exists(config.path)) {
        std::cout << console::red << "[ERROR] Cannot find file \"" << config.path << "\"."
                  << console::reset << std::endl;
        return 1;
    }

    try {
        ctb::engine::Window::run("CaptureTheBanana++", 1280, 720, config);
        return 0;
    } catch (const UserExceptionBaseT& ex) {
        std::cout << console::red << "[ERROR] [" << boost::core::demangle(typeid(ex).name()) << "] "
                  << ex.what() << console::reset << std::endl;
    } catch (const boost::property_tree::xml_parser_error& ex) {
        std::cout << console::red << "[ERROR] [xml_parser_error] " << ex.what() << console::reset
                  << std::endl;
    }
    return 1;
}

}  // namespace cli

#ifdef _WIN32

// TODO(felix): Use a Logger to output messages, otherwise all messages are gone...
// GUI entry point
// int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
//    return cli::run(__argc, __argv);
//}

// Console entry point
int wmain(int argc, wchar_t** wargv) {
    std::vector<std::string> arguments(argc);
    std::vector<const char*> argv(argc + 1);
    for (int i = 0; i < argc; ++i) {
        arguments[i] = common::utils::toUtf8(std::wstring(wargv[i]));
        argv[i] = arguments[i].c_str();
    }
    argv[argc] = nullptr;
    return cli::run(argc, const_cast<char**>(&argv[0]));
}

#else

int main(int argc, char** argv) {
    return cli::run(argc, argv);
}

#endif
