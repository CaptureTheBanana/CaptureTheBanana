// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <cmath>
#include <iostream>

#include <SDL.h>

#include "engine/Engine.hpp"
#include "engine/Window.hpp"
#include "engine/audio/SoundManager.hpp"
#include "engine/core/GC.hpp"
#include "engine/core/Game.hpp"
#include "engine/input/Input.hpp"
#include "engine/scene/Fist.hpp"
#include "engine/scene/Flag.hpp"
#include "engine/scene/Gun.hpp"
#include "engine/scene/Player.hpp"
#include "engine/util/Random.hpp"
#include "engine/util/Vector2d.hpp"

namespace ctb {
namespace engine {

Player::Player(Team team,
               SDL_Texture* texture,
               int animationWidth,
               int animationHeight,
               int animationCount)
    : ActingRenderable(texture, animationWidth, animationHeight, animationCount),
      m_right(false),
      m_left(false),
      m_score(250),
      m_running(false),
      m_shooting(false),
      m_health(100),
      m_team(team),
      m_hasFlag(false),
      m_lastSoundTicks(0),
      m_direction(Direction::Right),
      m_cooldownHeal(0),
      m_healingAnimationDone(false),
      m_nextMeleeTick(0) {
    m_input = nullptr;
    m_inputHandlerId = 0;
    m_angle = 0.0f;

    m_anim[PlayerAnimation::ANIM_GO] = std::pair<int, int>(2, 4);
    m_anim[PlayerAnimation::ANIM_AIM] = std::pair<int, int>(25, 1);
    m_anim[PlayerAnimation::ANIM_SHOOT] = std::pair<int, int>(26, 4);
    m_anim[PlayerAnimation::ANIM_JUMP] = std::pair<int, int>(40, 1);
    m_anim[PlayerAnimation::ANIM_STAND] = std::pair<int, int>(0, 1);
    m_anim[PlayerAnimation::ANIM_JUMP_ATTACK] = std::pair<int, int>(60, 3);
    m_anim[PlayerAnimation::ANIM_HEAL] = std::pair<int, int>(80, 9);
    m_anim[PlayerAnimation::ANIM_TAUNT] = std::pair<int, int>(89, 3);

    m_currAnimation = PlayerAnimation::ANIM_STAND;

    m_weapon = new Fist(nullptr, 30, 24, 1);
}

void Player::deregisterInput() {
    if (0 != m_inputHandlerId && m_input) {
        m_input->deregister_handler(m_inputHandlerId);
        m_inputHandlerId = 0;
    }
}

void Player::render() {
    if (m_left) {
        moveLeft();
    }

    if (m_right) {
        moveRight();
    }

    nextAnimation();
    if (onGround()) {
        playStepSound();
    }
    SDL_Rect target;

    Vector2dT position = computeTargetPosition();

    target.x = position.x;
    target.y = position.y;
    target.w = m_animationWidth;
    target.h = m_animationHeight;

    SDL_RendererFlip flip;
    if (!m_moveright) {
        flip = SDL_FLIP_HORIZONTAL;
    } else {
        flip = SDL_FLIP_NONE;
    }

    // Render current animation frame
    SDL_RenderCopyEx(Window::getWindow().renderer(), m_texture, &m_sourceRect, &target, 0, nullptr,
                     flip);

    if (m_weapon) {
        m_weapon->setOffset(m_offset);
        m_weapon->render();
    }
}

void Player::renderStatic(Vector2dT& pos) {
    SDL_Rect source, target;

    source.x = 0;
    source.y = 0;
    source.w = m_animationWidth;
    source.h = m_animationHeight;

    target.x = pos.x;
    target.y = pos.y;
    target.w = m_animationWidth;
    target.h = m_animationHeight;

    // Render current animation frame
    SDL_RenderCopy(Window::getWindow().renderer(), m_texture, &source, &target);
}

void Player::update() {
    if (m_weapon && !m_dropWeaponOnUpdate) {
        m_weapon->update();
    }

    if (m_dropWeaponOnUpdate) {
        m_dropWeaponOnUpdate = false;
        dropWeapon();
    }

    ActingRenderable::update();
}

void Player::setHasFlag(bool b) {
    m_hasFlag = b;
    if (b) {
        if (m_team == Team::R2L) {
            SoundManager::getInstance().playMusicRTL();
        } else {
            SoundManager::getInstance().playMusicLTR();
        }
    }
}

void Player::addToWorld(b2World& world, ActingKinematics& kinematics) {
    ActingRenderable::addToWorld(world, kinematics);

    // Add the weapon two the physical environment

    if (m_weapon) {
        Kinematics k(0.5f, 0.5f, 0.2f);
        k.setDensity(0.0f);
        k.setId(Game::WEAPON_ID);
        k.setCategory(Level::WEAPON_CAT);
        k.setMask(Level::PLAYER_CAT | Level::GROUND_CAT);
        m_weapon->addToWorld(world, k);
        m_weapon->setWorldPosition(this->worldPosition());
        m_weapon->setUser(this);
    }
}

void Player::heal() {
    Uint32 ticks = SDL_GetTicks();
    if (ticks > m_cooldownHeal && m_health < 100) {
        m_cooldownHeal = ticks + 10000;  // reset to ticks + 10 sec;
        m_health = std::min(m_health + kHpPerHeal, 100u);
        m_currAnimation = PlayerAnimation::ANIM_HEAL;
    }
}

void Player::addDamage(uint32_t damage, Player* damageFrom) {
    if (!alive()) {
        return;
    }
    if (damage == 0) {
        damage = 10u + static_cast<uint32_t>(Random::getInt(0, 5));
    }

    // If player is dying

    if (damage >= m_health) {
        SoundManager::getInstance().playDamage();
        m_health = 0;
        if (!m_game) {
            throw std::runtime_error("Cant respawn player because there is no game!");
        }
        m_game->setToRespawn(this);

        m_dropWeaponOnUpdate = true;
    } else {
        m_health -= damage;
    }

    if (damageFrom) {
        damageFrom->alterScore(PlayerScoreFrom::SCORE_PLAYER);
    }
}

void Player::reset() {
    PhysicalRenderable::reset();
    m_left = m_right = false;  // m_shooting = m_running = false;
    m_weapon->unuse();
    m_currAnimation = PlayerAnimation::ANIM_STAND;
}

void Player::doorReached() {
    if (!alive()) {
        return;
    }
    Window::getEngine().getGame()->nextLevel(m_team);
    alterScore(PlayerScoreFrom::SCORE_DOOR);
}

void Player::alterScore(PlayerScoreFrom from) {
    static uint64_t respawnCost = 50;

    switch (from) {
        case PlayerScoreFrom::SCORE_PLAYER:
            m_score += 100;
            break;
        case PlayerScoreFrom::SCORE_DOOR:
            m_score += (300 + respawnCost);
            break;
        case PlayerScoreFrom::SCORE_BOT:
            m_score += 100;
            break;
        case PlayerScoreFrom::SCORE_WON:
            m_score += 5000;
            break;
        case PlayerScoreFrom::SCORE_RESPAWN:
            m_score -= respawnCost > m_score ? m_score : respawnCost;
            break;
        case PlayerScoreFrom::SCORE_FLAG:
            m_score += 1;
            break;
        case PlayerScoreFrom::SCORE_PROJECTILE:
            m_score += 50;
            break;
    }
}

void Player::handleJumpAnimations() {
    if (!onGround()) {
        if (m_currAnimation == PlayerAnimation::ANIM_SHOOT ||
            m_currAnimation == PlayerAnimation::ANIM_JUMP_ATTACK) {
            m_currAnimation = PlayerAnimation::ANIM_JUMP_ATTACK;
        } else {
            m_currAnimation = PlayerAnimation::ANIM_JUMP;
        }
    } else {
        if (m_currAnimation == PlayerAnimation::ANIM_JUMP_ATTACK) {
            m_currAnimation = PlayerAnimation::ANIM_SHOOT;
        }
        if (m_currAnimation == PlayerAnimation::ANIM_JUMP) {
            if (m_left || m_right) {
                m_currAnimation = PlayerAnimation::ANIM_GO;
            } else {
                m_currAnimation = PlayerAnimation::ANIM_STAND;
            }
        }
    }
}

void Player::nextAnimation() {
    Uint32 ticks = SDL_GetTicks();
    float time = static_cast<float>(ticks - m_lastTick);

    if (time > static_cast<float>(m_animationDuration)) {
        handleJumpAnimations();

        if (isHealing() && !m_healingAnimationDone) {
            m_currAnimation = PlayerAnimation::ANIM_HEAL;
        }
        if (!isHealing()) {
            m_healingAnimationDone = false;
        }

        int firstFrame = m_anim[m_currAnimation].first;
        int numFrames = m_anim[m_currAnimation].second;

        // Check and increase frame counter
        if (m_currentAnimationStep < firstFrame) {
            m_currentAnimationStep = firstFrame;
        } else {
            if (m_currentAnimationStep + 1 < (firstFrame + numFrames)) {
                m_currentAnimationStep++;
            } else {
                if (m_currAnimation == PlayerAnimation::ANIM_HEAL) {
                    m_currAnimation = PlayerAnimation::ANIM_STAND;
                    m_healingAnimationDone = true;
                } else {
                    m_currentAnimationStep = firstFrame;
                }
            }
        }

        // Setup source rect
        m_sourceRect.x = m_currentAnimationStep * m_animationWidth;

        // Save current tick count
        m_lastTick = ticks;
    }
}

void Player::playStepSound() {
    Uint32 ticks = SDL_GetTicks();
    float time = static_cast<float>(ticks - m_lastSoundTicks);
    if (time > 200) {
        if (m_left || m_right) {
            SoundManager::getInstance().playStep();
        }

        // Save current tick count
        m_lastSoundTicks = ticks;
    }
}

void Player::registerInput(Input* input) {
    if (m_inputHandlerId == 0) {
        m_input = input;
        InputHandler handler =
            std::bind(&Player::handleInput, this, std::placeholders::_1, std::placeholders::_2,
                      std::placeholders::_3, std::placeholders::_4);
        m_inputHandlerId = input->register_handler(handler);
    }
}

void Player::jump() {
    SoundManager::getInstance().playJump();
    ActingRenderable::jump();
}

void Player::handleInput(InputType type, Input* /*input*/, bool state, float angle) {
    if (Window::getEngine().getGame()->getGameState() != GameState::Running || !alive()) {
        return;
    }

    switch (type) {
        case InputType::INPUT_PAUSE: {
            PauseMenu* pauseMenu = new PauseMenu(this);
            Window::getWindow().addMenu(pauseMenu);
        } break;
        case InputType::INPUT_LEFT:
            m_direction = Direction::Left;
            m_left = state;
            m_right = false;
            m_weapon->angleWeapon(static_cast<double>(m_angle), m_direction);
            break;
        case InputType::INPUT_RIGHT:
            m_direction = Direction::Right;
            m_right = state;
            m_left = false;
            m_weapon->angleWeapon(static_cast<double>(m_angle), m_direction);
            break;
        case InputType::INPUT_JUMP:
            if (state) {
                jump();
            }
            break;
        case InputType::INPUT_HEAL:
            if (state) {
                heal();
            }
            break;
        case InputType::INPUT_AIM_VERT:
            if (m_weapon) {
                m_angle = angle;
                m_weapon->angleWeapon(static_cast<double>(angle), m_direction);
            }
            break;
        case InputType::INPUT_DROP_WEAPON:
            if (state) {
                dropWeapon();
            }
            break;
        case InputType::INPUT_SHOOT:
            if (m_weapon) {
                if (state) {
                    if (m_weapon->inUse()) {
                        m_weapon->unuse();
                    } else {
                        m_weapon->use();
                    }
                } else {
                    m_weapon->unuse();
                }
            }
            break;
        default:
            break;
    }

    chooseAnimation(type, state);
}

void Player::chooseAnimation(InputType type, bool state) {
    if (type == InputType::INPUT_TEAM_1 || type == InputType::INPUT_TEAM_2 ||
        type == InputType::INPUT_UP || type == InputType::INPUT_DOWN ||
        type == InputType::INPUT_PAUSE || type == InputType::INPUT_SELECT ||
        type == InputType::INPUT_RETURN) {
        return;
    }

    if (state) {
        switch (type) {
            case InputType::INPUT_JUMP:
                // handeled in nextFrame, because a jumping player has only the jumping (and jump
                // attack) animation
                break;
            /*case InputType::INPUT_AIM_VERT:
                m_currAnimation = PlayerAnimation::ANIM_AIM;
                break;*/
            case InputType::INPUT_SHOOT:
                m_currAnimation = PlayerAnimation::ANIM_SHOOT;
                break;
            case InputType::INPUT_TAUNT:
                m_currAnimation = PlayerAnimation::ANIM_TAUNT;
                break;
            default:
                break;
        }
    } else {
        m_currAnimation = PlayerAnimation::ANIM_STAND;
    }

    if (type == InputType::INPUT_SHOOT) {
        m_shooting = state;
    }

    if (m_left || m_right) {
        if (m_shooting) {
            m_currAnimation = PlayerAnimation::ANIM_SHOOT;
            return;
        }
        m_currAnimation = PlayerAnimation::ANIM_GO;
    }
}

Player::~Player() {
    deregisterInput();

    if (m_weapon) {
        delete m_weapon;
        m_weapon = nullptr;
    }
}

void Player::collectWeapon(Fist* weapon) {
    if (m_weapon) {
        // We have already a non-default weapon, so ignore event.
        if (m_weapon->isDropable()) {
            return;
        }

        // Free current weapon and destroy box2d body.
        GC::add(m_weapon);
        m_weapon = nullptr;
    }

    // Remove weapon from spawn buffer and take ownership.
    m_game->getCurrentLevel()->removeWeapon(weapon);
    m_weapon = weapon;
    weapon->setUser(this);
    m_weapon->angleWeapon(static_cast<double>(m_angle), m_direction);
}

void Player::dropWeapon() {
    // If we have a droppable weapon drop it otherwise free memory.
    if (m_weapon) {
        m_weapon->setUser(nullptr);
        if (m_weapon->isDropable()) {
            m_game->getCurrentLevel()->dropWeapon(m_weapon);

        } else {
            GC::add(m_weapon);
        }
        m_weapon = nullptr;
    }

    m_weapon = new Fist(nullptr, 30, 24, 1);

    Kinematics k(0.5f, 0.5f, 0.2f);
    k.setDensity(0.0f);
    k.setId(Game::WEAPON_ID);
    k.setCategory(Level::WEAPON_CAT);
    k.setMask(Level::PLAYER_CAT | Level::GROUND_CAT);
    m_weapon->addToWorld(*(m_body->GetWorld()), k);

    m_weapon->setUser(this);
}
}  // namespace engine
}  // namespace ctb
