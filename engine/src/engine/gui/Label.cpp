// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <iostream>
#include <utility>

#include "engine/Window.hpp"
#include "engine/gui/Label.hpp"

namespace ctb {
namespace engine {

Label::Label(const std::string& fontName, std::string text, const Vector2dT& pos, int scale)
    : m_text(std::move(text)),
      m_char(static_cast<char>(0)),
      m_scale(scale),
      m_pos(pos),
      m_color(Color(255, 255, 255)),
      m_alignment(LabelAlignment::Left),
      m_show(true) {
    loadFont(fontName);
}

Label::Label(const std::string& fontName, char c, const Vector2dT& pos, int scale)
    : m_text(""),
      m_char(c),
      m_scale(scale),
      m_pos(pos),
      m_color(Color(255, 255, 255)),
      m_alignment(LabelAlignment::Left),
      m_show(true) {
    loadFont(fontName);
}

void Label::loadFont(const std::string& fontName) {
    // Get the font. Fail if the font was not found.
    m_font = Font::getFont(fontName);
    if (m_font == nullptr) {
        std::cout << "Cannot load font: " << fontName << std::endl;
        throw std::runtime_error("Font Exception");
    }
}

void Label::render() {
    // Just render, if the label is shown
    if (!m_show) {
        return;
    }

    // Set color
    SDL_Texture* texture = m_font->getTexture();
    SDL_SetTextureColorMod(texture, m_color.r, m_color.g, m_color.b);

    SDL_Rect target{};
    SDL_Rect source{};

    // Both have dimensions of one char.
    source.w = m_font->get_w();
    source.h = m_font->get_h();
    target.w = source.w * m_scale;
    target.h = source.h * m_scale;

    // Get the actual position of the first character with the alignment
    Vector2dT pos = m_pos;
    if (m_alignment == LabelAlignment::Center) {
        int textlen = static_cast<int>(m_text.length());
        int pixelWidth = textlen * target.w;
        pos.x = pos.x - pixelWidth / 2;
    } else if (m_alignment == LabelAlignment::Right) {
        int textlen = static_cast<int>(m_text.length());
        int pixelWidth = textlen * target.w;
        pos.x = pos.x - pixelWidth;
    }

    target.x = pos.x;
    target.y = pos.y;

    if (m_char) {
        renderChar(texture, target, source);
    } else {
        renderText(texture, target, source);
    }
}

void Label::renderChar(SDL_Texture* texture, SDL_Rect& target, SDL_Rect& source) {
    // Get the position in the character tilesheed based on the char value
    int col, row;
    uint32_t id = static_cast<uint32_t>(m_char);
    row = static_cast<int>(id) % 16;
    col = static_cast<int>(id) / 16;

    source.x = row * source.w;
    source.y = col * source.h;

    // Render the character
    SDL_RenderCopy(Window::getWindow().renderer(), texture, &source, &target);
}

void Label::renderText(SDL_Texture* texture, SDL_Rect& target, SDL_Rect& source) {
    int col, row;
    unsigned int id;
    // Iterate over all characters to render
    for (char& c : m_text) {
        id = static_cast<unsigned int>(c);
        row = static_cast<int>(id) % 16;
        col = static_cast<int>(id) / 16;

        source.x = row * source.w;
        source.y = col * source.h;

        // Render it!
        SDL_RenderCopy(Window::getWindow().renderer(), texture, &source, &target);

        target.x += source.w * m_scale;  // Move one char foreward
    }
}

}  // namespace engine
}  // namespace ctb
