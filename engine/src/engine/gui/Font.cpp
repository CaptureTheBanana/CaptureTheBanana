// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <fstream>
#include <iostream>

#include <common/Exceptions.hpp>
#include <common/Utils.hpp>

#include "engine/gui/Font.hpp"

namespace ctb {
namespace engine {

std::map<std::string, Font> Font::fontMap;

// Load all fonts in the given path. Fonts have to have the extension .fnt
void Font::loadFonts(const std::string& folderPath) {
    for (auto& file : boost::filesystem::directory_iterator(folderPath)) {
        boost::filesystem::path path(file);
        if (path.extension() == ".fnt") {
            Font::addFont(path.string(), path.stem().string());
        }
    }
}

// Add a fontFile to the static Font map
void Font::addFont(const boost::filesystem::path& path, const std::string& name) {
    Font f(path.string());
    fontMap.insert(std::pair<std::string, Font>(name, f));
}

// Query a Font and returns a pointer to it
Font* Font::getFont(const std::string& name) {
    auto it = fontMap.find(name);
    if (it == fontMap.end()) {
        return nullptr;
    }
    return &((*it).second);
}

Font::Font(const std::string& filename) {
    namespace cutils = ctb::common::utils;
    std::ifstream in(cutils::toOsPath(filename).c_str());
    if (!in.good()) {
        throw common::FileNotFoundException(cutils::toOsPath(filename));
    }

    // Get path with slash from given .fnt file
    std::string path = cutils::getDirectoryPathWithSlash(filename);

    // Parse the tilesheet name, the width and height of each character and color key
    std::string tilesheetFilename;
    in >> tilesheetFilename;

    int r, g, b;
    in >> m_w >> m_h >> r >> g >> b;

    // Cast keying colors manually!
    uint8_t keyR = static_cast<uint8_t>(r);
    uint8_t keyG = static_cast<uint8_t>(g);
    uint8_t keyB = static_cast<uint8_t>(b);

    // Load textures
    m_texture = Window::getWindow().loadTexture(path + tilesheetFilename, keyR, keyG, keyB);
}

}  // namespace engine
}  // namespace ctb
