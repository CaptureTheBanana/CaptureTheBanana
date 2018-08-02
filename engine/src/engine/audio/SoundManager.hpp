#ifndef ENGINE_AUDIO_SOUNDMANAGER_HPP
#define ENGINE_AUDIO_SOUNDMANAGER_HPP

#include <vector>

#include <SDL_mixer.h>
// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <parser/GameConfig.hpp>

#include "engine/audio/Music.hpp"
#include "engine/audio/SoundEffect.hpp"

namespace ctb {
namespace engine {

enum class TeamMusic { RTL, LTR, NONE };

/// Manages all music and sounds in the game
class SoundManager {
   public:
    SoundManager(SoundManager const&) = delete;

    SoundManager() = delete;

    void operator=(SoundManager const&) = delete;

    /// \brief initialize sound manager from gameconfig
    ///
    /// \brief config pointer to gameconfig
    /// \brief sound if sounds are enabled
    static void init(parser::GameConfig* config, bool sound);

    /// \brief delivers instance of SoundManager
    ///
    /// \return Soundmanager& instance
    /// \throws std::runtime_error if soundmanager has not been initialized yet
    static SoundManager& getInstance();

    /// deletes instance of Sound manager
    static void deleteSoundManager();

    /// Stops all background music
    void stopMusic();

    /// plays music for team L2R
    void playMusicLTR();

    /// plays music for team R2L
    void playMusicRTL();

    /// plays jump sound
    void playJump();

    /// plays step sound
    void playStep();

    /// plays door sound
    void playDoor();

    /// plays win sound
    void playWin();

    /// plays hit sound
    void playHit();

    /// plays damage sound
    void playDamage();

    /// plays pew sound
    void playPew();

    /// unregisters SDL Audio and deletes all sounds
    ~SoundManager();

   private:
    /// \brief initializes SDL Audio and loads all sounds from sounds folder
    ///
    /// \param config pointer to gameConfig that specifies the config folder
    /// \brief sound true if sounds should be enabled
    SoundManager(parser::GameConfig* config, bool sound);

    /// delivers the next channel to play audio on
    int getNextChannel();

    /// count of channels
    int m_channelCount{16};

    /// current channel on which audio gets played on
    int m_currentChannel{0};

    // musics
    /// music for team R2L
    Music* m_musicRTL{nullptr};
    /// music for team L2R
    Music* m_musicLTR{nullptr};

    // movements
    /// jump sound
    SoundEffect* m_jumpSound{nullptr};
    /// single step sound
    SoundEffect* m_stepSound{nullptr};
    /// door enter sound
    SoundEffect* m_doorSound{nullptr};
    /// win sound
    SoundEffect* m_winSound{nullptr};

    // weapons
    /// sound of player hit
    SoundEffect* m_hitSound{nullptr};
    // sound of player damage
    SoundEffect* m_damageSound{nullptr};
    /// sound of pew
    SoundEffect* m_pewSound{nullptr};

    /// Saves the music played for the current team or none
    TeamMusic m_teamMusic;
};

}  // namespace engine
}  // namespace ctb

#endif
