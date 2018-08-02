// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_UTIL_SDLDRIVER_HPP
#define ENGINE_UTIL_SDLDRIVER_HPP

#include <string>
#include <vector>

#include <SDL.h>
#include <gsl/gsl>

namespace ctb {
namespace engine {
namespace sdl_driver {

/// SDL render backend information
struct SdlRenderBackend {
    int index{-1};
    std::string name{};
    bool isSoftwareRenderer{false};
};

/// \brief Returns the current video driver name.
///
/// \return std::string Video driver name
std::string getVideoDriverName();

/// \brief Returns the current rendering backend name.
///
/// \param renderer Renderer
/// \return std::string Rendering backend name
std::string getRenderBackendName(gsl::not_null<SDL_Renderer*> renderer);

/// \brief Returns the rendering backend name from selected index.
///
/// \param index Index
/// \return std::string Rendering backend name
std::string getRenderBackendNameByIndex(int index);

/// Returns the selected render backend or -1 (default SDL value).
int getCurrentRenderBackend();

/// \brief Returns information about the given render backend.
///
/// \param index Render backend index
/// \param backend Backend information
/// \return bool true on success
bool getRenderBackendInfo(int index, SdlRenderBackend& backend);

/// Returns a list of available renderer backends.
std::vector<SdlRenderBackend> getRenderBackends();

}  // namespace sdl_driver
}  // namespace engine
}  // namespace ctb

#endif  // ENGINE_UTIL_SDLDRIVER_HPP
