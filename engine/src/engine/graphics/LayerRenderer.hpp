// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_GRAPHIC_LAYERRENDERER_HPP
#define ENGINE_GRAPHIC_LAYERRENDERER_HPP

#include <set>
#include <tuple>

#include <gsl/gsl>

namespace ctb {
namespace engine {

class Camera;

class TextureBasedRenderable;

/// Class that handles drawing of layers.
class LayerRenderer {
   public:
    using LayerT = std::tuple<TextureBasedRenderable*, int, bool>;

    /// Layer id comparator
    class LayerComparator {
       public:
        bool operator()(LayerT a, LayerT b) { return std::get<int>(a) < std::get<int>(b); }
    };

   public:
    /// \brief Creates a layer renderer with the given camera.
    ///
    /// \param camera Camera which sets the viewport - must stay alive until the class is
    ///               finished/killed.
    explicit LayerRenderer(gsl::not_null<Camera*> camera);

    /// Destructor
    virtual ~LayerRenderer();

    // Disable copy constructor and copy-assignment.
    LayerRenderer(const LayerRenderer&) = delete;

    LayerRenderer& operator=(const LayerRenderer&) = delete;

    /// \brief Adds a renderable to the given layer
    ///
    /// \param renderable Layer/Texture to render
    /// \param layerId Texture layer id
    /// \param freeTexture If true, we take ownership of renderable
    void addRenderable(TextureBasedRenderable* renderable, int layerId, bool freeRenderable);

    /// Renders each layer starting with layer id 0.
    void render();

   private:
    /// Pointer to the camera
    Camera* m_camera;
    /// List for the layers and information
    std::multiset<LayerT, LayerComparator> m_renderables;
};  // namespace ctb

}  // namespace engine
}  // namespace ctb

#endif  // ENGINE_GRAPHIC_LAYERRENDERER_HPP
