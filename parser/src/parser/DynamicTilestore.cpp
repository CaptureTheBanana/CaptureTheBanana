// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "parser/DynamicTilestore.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

namespace ctb {

DynamicTilestore::DynamicTilestore(int width, int height, int tileWidth, int tileHeight)
    : m_tileWidth(tileWidth), m_tileHeight(tileHeight) {
    m_tiles.resize(0);
    setHeight(height);
    setWidth(width);
}

DynamicTilestore::DynamicTilestore(const std::string& tileString,
                                   int width,
                                   int height,
                                   int tileWidth,
                                   int tileHeight)
    : m_tileWidth(tileWidth), m_tileHeight(tileHeight) {
    setHeight(height);
    setWidth(width);

    std::stringstream stream;
    stream << tileString;
    int id;
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            stream >> id;
            set(x, y, id);
        }
    }
}

int DynamicTilestore::get(int x, int y) const {
    return m_tiles.at(static_cast<size_t>(x)).at(static_cast<size_t>(y));
}

void DynamicTilestore::set(int x, int y, int id) {
    m_tiles.at(static_cast<size_t>(x)).at(static_cast<size_t>(y)) = id;
}

void DynamicTilestore::setWidth(int width) {
    while (m_width < width) {
        increaseWidth();
    }
    while (m_width > width) {
        decreaseWidth();
    }
}

void DynamicTilestore::increaseWidth() {
    std::vector<int> temp;
    temp.resize(static_cast<size_t>(m_height));
    m_tiles.push_back(temp);
    m_width++;
}

void DynamicTilestore::decreaseWidth() {
    m_tiles.pop_back();
    m_width--;
}

int DynamicTilestore::getWidth() {
    return m_width;
}

void DynamicTilestore::setHeight(int height) {
    while (m_height < height) {
        increaseHeight();
    }
    while (m_height > height) {
        decreaseHeight();
    }
}

void DynamicTilestore::increaseHeight() {
    for (vector<int>& vector : m_tiles) {
        vector.push_back(0);
    }
    m_height++;
}

void DynamicTilestore::decreaseHeight() {
    for (vector<int>& vector : m_tiles) {
        vector.pop_back();
    }
    m_height--;
}

int DynamicTilestore::getHeight() {
    return m_height;
}

int DynamicTilestore::getTileHeight() {
    return m_tileHeight;
}

int DynamicTilestore::getTileWidth() {
    return m_tileWidth;
}

void DynamicTilestore::flip() {
    for (int i = 0; i < m_width / 2; i++) {
        m_tiles.at(static_cast<size_t>(i)).swap(m_tiles.at(static_cast<size_t>(m_width - i - 1)));
    }
}

std::string DynamicTilestore::toTileString() {
    std::string result;
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            result += std::to_string(get(x, y));
            result += " ";
        }
    }
    return result;
}

}  // namespace ctb
