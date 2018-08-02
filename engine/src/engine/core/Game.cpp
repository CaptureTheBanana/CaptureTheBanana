// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <random>
#include <stdexcept>
#include <vector>

#include <Box2D/Box2D.h>
#include <SDL.h>
#include <boost/foreach.hpp>
#include <boost/range/join.hpp>
#include <gsl/gsl>
#include <parser/PlayerConfig.hpp>

#include "engine/Window.hpp"
#include "engine/audio/SoundManager.hpp"
#include "engine/core/Game.hpp"
#include "engine/input/Input.hpp"
#include "engine/menu/EndMenu.hpp"
#include "engine/scene/Door.hpp"
#include "engine/scene/Fist.hpp"
#include "engine/scene/Gun.hpp"
#include "engine/util/Random.hpp"
#include "engine/util/Vector2d.hpp"

using ctb::parser::LevelConfig;
using ctb::parser::LevelType;

namespace ctb {
namespace engine {

Game::Game(parser::GameConfig* config)
    : m_config(config),
      m_levelOrder(5),
      m_state(GameState::Stopped),
      m_currentLevel(2),
      m_statusbar(nullptr) {
    m_lastTicks = SDL_GetTicks();
    std::multimap<LevelType, LevelConfig*> lvls = config->getLevels();
    std::multimap<LevelType, LevelConfig*>::iterator st, end;

    // get end level
    int cnt = static_cast<int>(lvls.count(LevelType::END));
    if (cnt == 0) {
        throw std::runtime_error("No end Level!");
    }

    // Create b2worlds
    for (int i = 0; i < LEVELCOUNT; ++i) {
        m_levelWorlds.push_back(new LevelWorld(b2Vec2(0, /*1000*/ 15)));
    }

    std::tie(st, end) = lvls.equal_range(LevelType::END);
    std::advance(st, Random::getInt(0, cnt - 1));  // advance to a random end level
    m_levelOrder[0] = new Level(config, (*st).second, m_levelWorlds.at(0));
    m_levelOrder[LEVELCOUNT - 1] =
        new Level(config, new LevelConfig(*(*st).second), m_levelWorlds.at(LEVELCOUNT - 1), true);

    // get center level
    cnt = static_cast<int>(lvls.count(LevelType::CENTER));
    if (cnt == 0) {
        throw std::runtime_error("No center Level!");
    }
    std::tie(st, end) = lvls.equal_range(LevelType::CENTER);
    std::advance(st, Random::getInt(0, cnt - 1));  // advance to a random center level
    m_levelOrder[LEVELCOUNT / 2] =
        new Level(config, (*st).second, m_levelWorlds.at(LEVELCOUNT / 2));

    // get levels inbetween
    cnt = static_cast<int>(lvls.count(LevelType::DEFAULT));
    // there has to be enough default levels, e.g. at least one if LEVELCOUNT=5
    int defCnt = (LEVELCOUNT - 1) / 2 - 1;
    if (cnt < defCnt) {
        throw std::runtime_error("Not enough default Levels!");
    }
    // to have unique levels, shuffle a vector with all possible numbers and pick the first x
    std::vector<int> v(static_cast<unsigned>(defCnt));
    std::iota(v.begin(), v.end(), 0);
    std::shuffle(v.begin(), v.end(), std::mt19937(std::random_device()()));
    for (size_t i = 0; i < static_cast<size_t>(defCnt); i++) {
        std::tie(st, end) = lvls.equal_range(LevelType::DEFAULT);
        std::advance(st,
                     v.at(i));  // get the level corresponding to the current unique random number
        m_levelOrder[i + 1] = new Level(config, (*st).second, m_levelWorlds.at(i + 1));
        m_levelOrder[LEVELCOUNT - i - 2] = new Level(config, new LevelConfig(*(*st).second),
                                                     m_levelWorlds.at(LEVELCOUNT - i - 2), true);
    }

    // play Soundtrack
    if (Random::getBool()) {
        SoundManager::getInstance().playMusicRTL();
    } else {
        SoundManager::getInstance().playMusicLTR();
    }
}

void Game::render() {
    m_levelOrder[m_currentLevel]->render();
    if (m_statusbar && m_state == GameState::Running) {
        m_statusbar->render();
    }
}

void Game::update() {
    if (m_state == GameState::Running) {
        m_levelOrder[m_currentLevel]->update();
    }

    // check for player respawns
    auto& cam = m_levelOrder[m_currentLevel]->getCamera();
    for (Player* a : boost::join(m_players_L2R, m_players_R2L)) {
        if (a->alive() && cam.checkBounds(a)) {
            a->addDamage(5);  // Nice effect of counting the damage down (it's fast ;))
        }
    }

    // iterate through respawnQueue, decrement counters and respawn players if necessary
    Uint32 ticks = SDL_GetTicks();
    std::vector<Player*> del;  // temp vector because iterator gets invalidated on delete ._.
    for (auto& pair : m_respawnQueue) {
        Player* a = pair.first;
        auto& val = pair.second;
        a->setPosition(Vector2dT(-100, 10000));  // just render far, far away
        a->reset();

        if (a->hasFlag() && val.second) {  // if he had flag and died, set flag free
            a->alterScore(PlayerScoreFrom::SCORE_RESPAWN);
            a->destroyJoint();
            a->setHasFlag(false);
            m_levelOrder[m_currentLevel]->getFlag()->setInUse(false);
        }

        val.first -= static_cast<int32_t>(ticks - m_lastTicks);
        if (val.first < 0) {
            del.push_back(a);
        }
    }
    for (auto& d : del) {
        respawnPlayer(d);
        m_respawnQueue.erase(d);
    }

    // increase score of team with flag
    for (Player* player : m_players_R2L) {
        if (player->hasFlag()) {
            flagScore(Team::R2L);
            break;
        }
    }

    for (Player* player : m_players_L2R) {
        if (player->hasFlag()) {
            flagScore(Team::L2R);
            break;
        }
    }

    m_lastTicks = ticks;
}

void Game::flagScore(Team hasFlag) {
    if (m_state != GameState::Running) {
        return;
    }

    if (hasFlag == Team::L2R) {
        for (Player* player : m_players_L2R) {
            player->alterScore(PlayerScoreFrom::SCORE_FLAG);
        }
    } else {
        for (Player* player : m_players_R2L) {
            player->alterScore(PlayerScoreFrom::SCORE_FLAG);
        }
    }
}

void Game::nextLevel(Team t) {
    int nextLevel = static_cast<signed>(m_currentLevel);
    nextLevel += (t == Team::L2R) ? 1 : -1;
    if (nextLevel < 0 || nextLevel >= LEVELCOUNT) {  // game finished
        SoundManager::getInstance().stopMusic();
        SoundManager::getInstance().playWin();
        endGame(t);
    } else {
        SoundManager::getInstance().playDoor();
        startLevel(static_cast<unsigned>(nextLevel), &t);
    }
}

void Game::startLevel(uint32_t level, Team* t) {
    m_levelOrder[m_currentLevel]->getFlag()->setInUse(false);
    m_currentLevel = level;
    Level* lvl = m_levelOrder[m_currentLevel];
    // make temp map with all players and corresponding teams
    auto all_players = std::map<Team, std::vector<Player*>>{{Team::L2R, m_players_L2R},
                                                            {Team::R2L, m_players_R2L}};
    Player* aWithFlag = nullptr;

    // if one team triggered the level change, do:
    // 1. start rendering at the opponents door in this level
    // 2. bind the flag of this level to the player that carried it to the door
    if (t) {
        auto it = std::find_if(lvl->getDoors().begin(), lvl->getDoors().end(),
                               [&t](Door* d) { return d->getTeam() != *t; });
        if (it == lvl->getDoors().end()) {
            throw std::logic_error("WTF NO DOOR ON THIS TEAM");
        }
        lvl->getCamera().focusDoor(*it);  // temp. focus door
        auto& acs = all_players.at(*t);
        auto pl = std::find_if(acs.begin(), acs.end(), [](Player* a) { return a->hasFlag(); });
        if (pl == acs.end()) {
            throw std::logic_error("No player on this team has the flag!");
        }
        aWithFlag = *pl;
        addPlayerToCurrentLevel(aWithFlag);

        Gun* gun = dynamic_cast<Gun*>(aWithFlag->getWeapon());

        if (gun) {
            gun->removeAllProjectiles();
        }

        lvl->getCamera().setFocus(aWithFlag);  // temp. focus player
    }

    // add players
    for (auto pair : all_players) {
        for (Player* player : pair.second) {
            if (player == aWithFlag) {
                continue;
            }
            addPlayerToCurrentLevel(player);

            Gun* gun = dynamic_cast<Gun*>(player->getWeapon());

            if (gun) {
                gun->removeAllProjectiles();
            }

            if (player->hasFlag()) {
                aWithFlag = player;
                lvl->getCamera().setFocus(player);
            }
        }
    }
    // respawn flag and bind to player if necessary
    lvl->respawnFlag();
    if (aWithFlag) {
        lvl->getFlag()->setInUse(true);
        lvl->getFlag()->setPosition(Vector2dT(aWithFlag->position()));
        lvl->getFlag()->createJoint(aWithFlag);
    }
    lvl->getCamera().setFocus(lvl->getFlag());  // reset focus to flag
}

void Game::addPlayerToCurrentLevel(Player* player) {
    m_levelOrder[m_currentLevel]->addPlayer(player, m_config->getPlayerLayer());
    respawnPlayer(player);
}

void Game::setToRespawn(Player* player) {
    setToRespawn(player, RESPAWN_TIMEOUT);
}

void Game::setToRespawn(Player* player, const int timeout, const bool died) {
    if (m_respawnQueue.count(player) == 0) {
        m_respawnQueue.insert(std::make_pair(player, std::make_pair(timeout, died)));
    }
}

void Game::respawnPlayer(Player* player) {
    if (player->getBody()->GetWorld()->IsLocked()) {
        setToRespawn(player, 0, false);
        return;
    }

    // find next spawn point and respawn player there
    auto* lvl = m_levelOrder[m_currentLevel];
    std::vector<int> pos = getNextPlayerSpawn(lvl, player->getTeam());
    if (pos.empty()) {
        throw std::runtime_error("Unsufficent spawn points!");
    }

    Vector2dT worldPos = lvl->getConfig()->toWorld(Vector2dT(pos[0], pos[1]));
    player->resetOnGround();
    player->reset();
    player->setPosition(Vector2dT(worldPos.x, worldPos.y - m_config->getPlayerOffset() - 2));
    player->setFullHealth();
    player->alterScore(PlayerScoreFrom::SCORE_RESPAWN);
}

std::vector<int> Game::getNextPlayerSpawn(Level* level, Team team) {
    auto& cam = level->getCamera();
    auto* config = level->getConfig();
    int x =
        (cam.x() + (team == Team::R2L ? 1 : -1) * (cam.width() / 2 - 40)) / config->getTileWidth();

    // This is a workaround to normalize the camera viewpoint.
    // |          |             |         |
    // s s s f s s s  becomes  s s s f s s s
    if (team == Team::L2R) {
        ++x;
    }

    int step = (team == Team::L2R) ? 1 : -1;
    for (; x != cam.x() / config->getTileWidth() && x >= cam.minX() / config->getTileWidth() &&
           x < cam.maxX() / config->getTileWidth();
         x += step) {
        auto& ys = config->getPlayerSpawns().at(static_cast<size_t>(x));
        if (!ys.empty()) {
            int y_out = 0;
            int min = 0;
            for (size_t i = 0; i < ys.size(); i++) {
                int newMin = abs(static_cast<int>(
                    ys.at(i) - static_cast<uint32_t>(cam.y() / config->getTileHeight())));
                if (i == 0 || newMin < min) {
                    min = newMin;
                    y_out = static_cast<int>(ys.at(i));
                }
            }
            return {x, y_out};
        }
    }
    return std::vector<int>();
}

// Creates all players needed.
void Game::startGame(std::vector<Player*> players) {
    if (m_state == GameState::Stopped) {
        for (auto& player : players) {
            // Put the player in the right team-vector.
            if (player->getTeam() == Team::R2L) {
                m_players_R2L.push_back(player);
            } else {
                m_players_L2R.push_back(player);
            }
            player->setGame(this);
        }

        // Init statusbar
        m_statusbar = new Statusbar();
        startLevel(m_currentLevel);
        m_state = GameState::Running;
    }
}

void Game::endGame(Team teamWon) {
    std::vector<Player*>* winningPlayers{nullptr};
    if (teamWon == Team::R2L) {
        winningPlayers = &m_players_R2L;
    } else if (teamWon == Team::L2R) {
        winningPlayers = &m_players_L2R;
    } else {
        std::logic_error("There is no winning team!");
    }

    Ensures(winningPlayers != nullptr);
    for (Player* a : *winningPlayers) {
        a->alterScore(PlayerScoreFrom::SCORE_WON);
    }

    pause();
    EndMenu* menu = new EndMenu(teamWon);
    Window::getWindow().addMenu(menu);
}

void Game::pause() {
    if (m_state == GameState::Running) {
        m_state = GameState::Paused;
    }
}

void Game::resume() {
    if (m_state == GameState::Paused) {
        m_state = GameState::Running;
    }
}

Game::~Game() {
    for (Level* lvl : m_levelOrder) {
        delete lvl;
        lvl = nullptr;
    }

    delete m_statusbar;
    m_statusbar = nullptr;

    for (Player* a : boost::join(m_players_L2R, m_players_R2L)) {
        delete a;
        a = nullptr;
    }

    // Destroy b2worlds
    for (auto* world : m_levelWorlds) {
        delete world;
        world = nullptr;
    }
}

}  // namespace engine
}  // namespace ctb
