// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/audio/SoundManager.hpp"

#ifndef ENABLE_SOUND

namespace ctb {
namespace engine {

SoundManager* instance = nullptr;

void SoundManager::init(parser::GameConfig* /*config*/, bool /*sound*/) {
    instance = new SoundManager(nullptr, false);
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

SoundManager::SoundManager(parser::GameConfig* /*config*/, bool /*sound*/) {}

void SoundManager::stopMusic() {}

void SoundManager::playMusicRTL() {}

void SoundManager::playMusicLTR() {}

void SoundManager::playJump() {}

void SoundManager::playStep() {}

void SoundManager::playDoor() {}

void SoundManager::playWin() {}

void SoundManager::playHit() {}

void SoundManager::playPew() {}

void SoundManager::playDamage() {}

SoundManager::~SoundManager() {}

int SoundManager::getNextChannel() {
    return 0;
}

}  // namespace engine
}  // namespace ctb

#endif  // ENABLE_SOUND
