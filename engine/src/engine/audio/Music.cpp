// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifdef ENABLE_SOUND
#include <iostream>

#include "engine/audio/Music.hpp"

namespace ctb {
namespace engine {

Music::Music(const std::string& filename) {
    m_music = Mix_LoadMUS(filename.c_str());
    if (!m_music) {
        std::cout << filename << std::endl;
        throw std::invalid_argument("unable to load the Music file");
    }
}

void Music::play() {
    if (Mix_PlayMusic(m_music, -1) == -1) {
        throw std::runtime_error("unable to play the Music");
    }
}

void Music::stop() {
    Mix_HaltMusic();
}

void Music::setVolume(int volume) {
    Mix_VolumeMusic(volume);
}

Music::~Music() {
    if (m_music) {
        Mix_FreeMusic(m_music);
    }
}

}  // namespace engine
}  // namespace ctb
#endif
