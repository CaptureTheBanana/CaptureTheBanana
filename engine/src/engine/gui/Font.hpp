// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_GUI_FONT_H
#define ENGINE_GUI_FONT_H

#include <map>
#include <string>

#include <SDL.h>
#include <boost/filesystem.hpp>

#include "engine/Window.hpp"

namespace ctb {
namespace engine {

/// \brief This class is a manager for all fonts. A font is represented by a .fnt file
///        with the tilesheet filename, the character width and height and the keying color.
///        Each character tilesheet has to be 16x16 chaaracters to cover the ascii range plus
///        an 8 bit extension.
class Font {
   public:
    /// \brief Searches all fonts (extension .fnt) in the given folder and loads them. The
    /// filename is used (without extension) to get the font later on.
    /// \param std::string folderPath The folder to search for fonts
    static void loadFonts(const std::string& folderPath);

    /// \brief Add a font to the fontMap and make it available to render
    /// boost::filesystem::path path The path to the .fnt file
    /// std::string name The name of the font. Needed to insert into the font map. The
    /// font is available with this name.
    static void addFont(const boost::filesystem::path& path, const std::string& name);

    /// \brief Queries a font from the fontMap.
    /// \param std::string name The name of the font to look up.
    /// \return A pointer to the font or a nullptr, if the font was not found.
    static Font* getFont(const std::string& name);

    /// \brief Returns the texture for rendering
    /// \returns The font texture
    SDL_Texture* getTexture() const { return m_texture; }

    /// \brief Get the width of each character
    /// \returns the width
    int get_w() const { return m_w; }

    /// \brief Get the height of each character
    /// \returns the height
    int get_h() const { return m_h; }

   private:
    /// Maps all font names to loaded fonts.
    static std::map<std::string, Font> fontMap;

    /// \brief private contructor, so only the static functions can add fonts
    /// \param std::string filename The name of the font
    /// \throws std::runtime_error if the found could not be loaded
    explicit Font(const std::string& filename);

    /// A font's texture
    SDL_Texture* m_texture;

    /// The height of each character
    int m_h;

    /// the width of each charackter
    int m_w;
};

}  // namespace engine
}  // namespace ctb
#endif
