// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/graphics/TextureBasedRenderable.hpp"
#include "engine/Window.hpp"

namespace ctb {
namespace engine {

TextureBasedRenderable::TextureBasedRenderable(SDL_Texture* texture)
    : m_position(0, 0),
      m_texture(texture),
      m_offset(0, 0),
      m_windowOffset(Window::getWindow().w() / 2, Window::getWindow().h() / 2),
      m_flip(SDL_FLIP_NONE),
      m_flip_angle(0.0) {
    m_sourceRect = {0, 0, 0, 0};
    m_targetRect = {0, 0, 0, 0};

    if (m_texture != nullptr) {
        Uint32 format;
        int access, w, h;
        SDL_QueryTexture(m_texture, &format, &access, &w, &h);
        m_sourceRect.w = w;
        m_sourceRect.h = h;
        m_targetRect.w = w;
        m_targetRect.h = h;
    }
}

void TextureBasedRenderable::render() {
    if (m_texture != nullptr) {
        SDL_RenderCopyEx(Window::getWindow().renderer(), m_texture, &m_sourceRect, &m_targetRect,
                         m_flip_angle, nullptr, m_flip);
    }
}

void TextureBasedRenderable::setPosition(const Vector2dT& v) {
    m_position = v;
    m_targetRect.x = m_position.x;
    m_targetRect.y = m_position.y;
}

Vector2dT TextureBasedRenderable::computeTargetPosition() const {
    return Vector2dT(m_position - m_offset + m_windowOffset);
}

TextureBasedRenderable::~TextureBasedRenderable() {
    if (m_texture != nullptr) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}

}  // namespace engine
}  // namespace ctb
