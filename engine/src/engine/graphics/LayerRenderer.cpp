// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <stdexcept>

#include "engine/core/Camera.hpp"
#include "engine/graphics/LayerRenderer.hpp"
#include "engine/graphics/TextureBasedRenderable.hpp"

namespace ctb {
namespace engine {

LayerRenderer::LayerRenderer(gsl::not_null<Camera*> camera) : m_camera(camera) {}

LayerRenderer::~LayerRenderer() {
    for (auto& item : m_renderables) {
        if (std::get<bool>(item)) {
            delete std::get<TextureBasedRenderable*>(item);
        }
    }
}

void LayerRenderer::addRenderable(TextureBasedRenderable* renderable,
                                  int layerId,
                                  bool freeRenderable) {
    Expects(renderable != nullptr);
    m_renderables.insert(std::make_tuple(renderable, layerId, freeRenderable));
}

void LayerRenderer::render() {
    if (!m_camera) {
        throw std::runtime_error("No camera defined in LayerManager!");
    }

    for (auto& item : m_renderables) {
        auto* renderable = std::get<TextureBasedRenderable*>(item);
        Expects(renderable != nullptr);
        renderable->setOffset(m_camera->getPosition());
        renderable->render();
    }
}

}  // namespace engine
}  // namespace ctb
