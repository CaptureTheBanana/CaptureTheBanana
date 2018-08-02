// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifdef ENABLE_SOUND
#include <stdexcept>

#include "engine/audio/SoundEffect.hpp"

namespace ctb {
namespace engine {

SoundEffect::SoundEffect(const std::string& filename) {
    m_effect = Mix_LoadWAV(filename.c_str());
    if (!m_effect) {
        throw std::invalid_argument("unable to load the Sound file");
    }
    setVolume(128);
}

void SoundEffect::play(int channel) {
    Mix_Volume(channel, m_volume);

    if (Mix_PlayChannel(channel, m_effect, 0) == -1) {
        throw std::runtime_error("unable to play the Sound");
    }
}

void SoundEffect::setVolume(int volume) {
    m_volume = volume;
}

SoundEffect::~SoundEffect() {
    if (m_effect) {
        Mix_FreeChunk(m_effect);
    }
}

}  // namespace engine
}  // namespace ctb
#endif
