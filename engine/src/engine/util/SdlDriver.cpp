// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/util/SdlDriver.hpp"
#include "engine/util/Exceptions.hpp"

namespace ctb {
namespace engine {
namespace sdl_driver {

std::string getVideoDriverName() {
    const auto* name = SDL_GetCurrentVideoDriver();
    if (name == nullptr) {
        return "n.a.";
    }
    return name;
}

std::string getRenderBackendName(gsl::not_null<SDL_Renderer*> renderer) {
    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(renderer, &info) != 0) {
        return "n.a.";
    }
    return {info.name};
}

std::string getRenderBackendNameByIndex(int index) {
    if (index < -1) {
        return "n.a.";
    } else if (index == -1) {
        return "default";
    }

    auto backends = getRenderBackends();
    if (static_cast<size_t>(index) >= backends.size()) {
        return "n.a.";
    }
    return backends.at(static_cast<size_t>(index)).name;
}

int getCurrentRenderBackend() {
    const auto* var = std::getenv("CTB_RENDER_BACKEND");
    if (var == nullptr) {
        return -1;
    }

    char* status = nullptr;
    int index = gsl::narrow<int>(std::strtol(var, &status, 10));
    if (status == nullptr || index <= -1) {
        return -1;
    }

    SdlRenderBackend info{};
    if (!getRenderBackendInfo(index, info)) {
        return -1;
    }
    return index;
}

bool getRenderBackendInfo(int index, SdlRenderBackend& backend) {
    SDL_RendererInfo info;
    if (SDL_GetRenderDriverInfo(index, &info) != 0) {
        return false;
    }

    backend.index = index;
    backend.name = info.name;
    backend.isSoftwareRenderer = (info.flags & SDL_RENDERER_SOFTWARE) == SDL_RENDERER_SOFTWARE;
    return true;
}

std::vector<SdlRenderBackend> getRenderBackends() {
    std::vector<SdlRenderBackend> drivers{};
    for (int i = 0; i < SDL_GetNumRenderDrivers(); ++i) {
        SdlRenderBackend info{};
        if (!getRenderBackendInfo(i, info)) {
            throw SdlException("SDL_GetRenderDriverInfo failed", SDL_GetError());
        }
        drivers.push_back(info);
    }
    return drivers;
}

}  // namespace sdl_driver
}  // namespace engine
}  // namespace ctb
