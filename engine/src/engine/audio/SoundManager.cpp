// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/audio/SoundManager.hpp"

#ifdef ENABLE_SOUND
namespace ctb {
namespace engine {

SoundManager* instance = nullptr;

void SoundManager::init(parser::GameConfig* config, bool sound) {
    // Create one instance
    instance = new SoundManager(config, sound);
}

SoundManager& SoundManager::getInstance() {
    if (!instance) {
        throw std::runtime_error("SoundManager has not been initialized yet");
    }
    return *instance;
}

void SoundManager::deleteSoundManager() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

SoundManager::SoundManager(parser::GameConfig* config, bool sound) {
    // TODO(felix): Currently disabled, because of missing mp3 library in SDL2_mixer. At the the
    //              moment we use the WAV format.
    // if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
    //    throw std::runtime_error("Mix_Init failed: " + std::string(Mix_GetError()));
    //}

    // Initialize SDL_mixer
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        throw std::runtime_error("Cannot open Audio: " + std::string(Mix_GetError()));
    }

    Mix_AllocateChannels(m_channelCount);

    // Current played music
    m_teamMusic = TeamMusic::NONE;

    if (sound) {
        Music::setVolume(32);
        m_musicRTL = new Music(config->getSoundsFolder() + "musicRTL.wav");
        m_musicLTR = new Music(config->getSoundsFolder() + "musicLTR.wav");

        m_stepSound = new SoundEffect(config->getSoundsFolder() + "step.wav");
        m_jumpSound = new SoundEffect(config->getSoundsFolder() + "jump.wav");
        m_doorSound = new SoundEffect(config->getSoundsFolder() + "door.wav");
        m_winSound = new SoundEffect(config->getSoundsFolder() + "win.wav");

        m_hitSound = new SoundEffect(config->getSoundsFolder() + "hit.wav");
        m_hitSound->setVolume(96);
        m_damageSound = new SoundEffect(config->getSoundsFolder() + "damage.wav");
        m_pewSound = new SoundEffect(config->getSoundsFolder() + "pew.wav");
    }
}

void SoundManager::stopMusic() {
    m_teamMusic = TeamMusic::NONE;
    Music::stop();
}

void SoundManager::playMusicRTL() {
    if (m_musicRTL && m_teamMusic != TeamMusic::RTL) {
        stopMusic();
        m_musicRTL->play();
        m_teamMusic = TeamMusic::RTL;
    }
}

void SoundManager::playMusicLTR() {
    if (m_musicLTR && m_teamMusic != TeamMusic::LTR) {
        stopMusic();
        m_musicLTR->play();
        m_teamMusic = TeamMusic::LTR;
    }
}

void SoundManager::playJump() {
    if (m_jumpSound) {
        m_jumpSound->play(getNextChannel());
    }
}

void SoundManager::playStep() {
    if (m_stepSound) {
        m_stepSound->play(getNextChannel());
    }
}

void SoundManager::playDoor() {
    if (m_doorSound) {
        m_doorSound->play(getNextChannel());
    }
}

void SoundManager::playWin() {
    if (m_winSound) {
        m_winSound->play(getNextChannel());
    }
}

void SoundManager::playHit() {
    if (m_hitSound) {
        m_hitSound->play(getNextChannel());
    }
}

void SoundManager::playDamage() {
    if (m_damageSound) {
        m_damageSound->play(getNextChannel());
    }
}

void SoundManager::playPew() {
    if (m_pewSound) {
        m_pewSound->play(getNextChannel());
    }
}

SoundManager::~SoundManager() {
    delete m_musicRTL;
    delete m_musicLTR;

    delete m_stepSound;
    delete m_jumpSound;
    delete m_doorSound;
    delete m_winSound;

    delete m_hitSound;
    delete m_damageSound;
    delete m_pewSound;

    // quit SDL_mixer
    Mix_CloseAudio();
}

int SoundManager::getNextChannel() {
    int channel = m_currentChannel++;
    if (m_currentChannel >= m_channelCount) {
        m_currentChannel = 0;
    }
    return channel;
}

}  // namespace engine
}  // namespace ctb

#endif  // ENABLE_SOUND
