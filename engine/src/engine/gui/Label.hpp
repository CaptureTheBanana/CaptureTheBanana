// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_GUI_LABEL_H
#define ENGINE_GUI_LABEL_H

#include <SDL.h>
#include <string>

#include "engine/graphics/SDLRenderable.hpp"
#include "engine/gui/Font.hpp"
#include "engine/util/Color.hpp"
#include "engine/util/Vector2d.hpp"

namespace ctb {
namespace engine {

class Font;

/// The alignment to the given position for rendering
enum class LabelAlignment { Left, Center, Right };

/// \brief Represents a renderable label that renders the given string or character to the position
class Label : public SDLRenderable {
   public:
    /// Renders the given text.
    Label(const std::string& fontName, std::string text, const Vector2dT& pos, int scale = 1);

    /// Renders the given char. The text has no influence now.
    Label(const std::string& fontName, char c, const Vector2dT& pos, int scale = 1);

    /// Setter for text
    void setText(const std::string text) { m_text = text; }

    /// Setter for pos
    void setPosition(Vector2dT pos) { m_pos = pos; }

    /// Setter for the scale
    void setScale(int scale) { m_scale = scale; }

    /// Setter for color
    void setColor(Color c) { m_color = c; }

    /// Setter for the alignment for rendering
    void setAlignment(LabelAlignment a) { m_alignment = a; }

    /// Toggle show
    void toggleShow() { m_show = !m_show; }

    /// Setter, if the label should be rendered
    void setShow(const bool s) { m_show = s; }

    /// renders the label
    void render() override;

    ~Label() override = default;

   private:
    /// Laod m_font from the given fontName
    void loadFont(const std::string& fontName);

    /// Rendern m_char
    void renderChar(SDL_Texture* texture, SDL_Rect& target, SDL_Rect& source);

    /// Renders m_text
    void renderText(SDL_Texture* texture, SDL_Rect& target, SDL_Rect& source);

    /// The font
    Font* m_font;

    /// The text to render. Will be ignored, if m_char is set.
    std::string m_text;

    /// A single char to render. If this is set, m_text won't be rendered.
    char m_char;

    /// A scale for the font.
    int m_scale;

    /// The position to render to.
    Vector2dT m_pos;

    /// The color to render
    Color m_color;

    /// The alignment
    LabelAlignment m_alignment;

    /// Whether this label should be rendered
    bool m_show;
};

}  // namespace engine
}  // namespace ctb

#endif
