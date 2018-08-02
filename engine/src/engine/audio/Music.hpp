// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_AUDIO_MUSIC_HPP
#define ENGINE_AUDIO_MUSIC_HPP

#ifdef ENABLE_SOUND
#include <string>

#include <SDL_mixer.h>

namespace ctb {
namespace engine {

/// In-game music
class Music {
   public:
    /// \brief creates music from file
    ///
    /// \param filename file of music
    /// \throws std::invalid_argument if music cannot be load from file
    explicit Music(const std::string& filename);

    /// \brief plays Music on repeat
    ///         if other music plays already only this music will play after calling play()
    ///
    /// \throws std::runtime_error if music cannot be played
    void play();

    /// stops Music
    static void stop();

    /// \brief changes volume of music from 0 to 128
    ///         affects currently playing music
    ///
    /// \param volume new volume of music
    static void setVolume(int volume);

    /// Destructor
    virtual ~Music();

   private:
    /// sdl music
    Mix_Music* m_music;
};

}  // namespace engine
}  // namespace ctb

#else

namespace ctb {
namespace engine {

class Music {
    virtual ~Music() {}
};

}  // namespace engine
}  // namespace ctb

#endif  // ENABLE_SOUND
#endif
