// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_AUDIO_SOUNDEFFECT_HPP
#define ENGINE_AUDIO_SOUNDEFFECT_HPP

#ifdef ENABLE_SOUND

#include <string>

#include <SDL_mixer.h>

namespace ctb {
namespace engine {

/// Soundeffects for weapons, damage, jump, ...
class SoundEffect {
   public:
    /// \brief creates sound effect from file
    ///
    /// \param filename sound file
    /// \throws std::invalid_argument if file cannot be load
    explicit SoundEffect(const std::string& filename);

    /// \brief plays sound once
    ///
    /// \param channel channel of sound
    /// \throws std::runtime_error if sound cannot be played
    void play(int channel);

    /// \brief sets volume
    ///         doesnt affect currently playung sounds
    ///
    /// \param volume volume from (0-100)
    void setVolume(int volume);

    /// Destructor
    virtual ~SoundEffect();

   private:
    /// sdl sound effect
    Mix_Chunk* m_effect{nullptr};

    // volume of sound
    int m_volume{100};
};

}  // namespace engine
}  // namespace ctb

#else
namespace ctb {
class SoundEffect {
    virtual ~SoundEffect() {}
};
}  // namespace ctb

#endif
#endif
