// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <iostream>
#include <stdexcept>

#include <boost/filesystem.hpp>

#include "engine/Engine.hpp"
#include "engine/gui/Label.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/input/Keyboard.hpp"
#include "engine/menu/CreditsMenu.hpp"
#include "engine/menu/HighscoreMenu.hpp"
#include "engine/menu/StartMenu.hpp"
#include "engine/util/Color.hpp"
#include "engine/util/Vector2d.hpp"

namespace ctb {
namespace engine {

StartMenu::StartMenu(parser::GameConfig* config)
    : m_gameconfig(config),
      m_lastRenderTicks(0),
      m_selected(0),
      m_startEnabled(true),
      m_freePlayersUsed(true) {
    setFPS(4);  // blick with 4 fps
    setupPlayers();
    setupUI();
}

void StartMenu::setupPlayers() {
    uint32_t max_players = static_cast<uint32_t>(m_gameconfig->getPlayers().size());
    Player* player = nullptr;
    parser::PlayerConfig* config;

    // Create all players.
    for (uint32_t i = 0; i < max_players; i++) {
        // get the config
        config = m_gameconfig->getPlayers()[i];

        player =
            new Player(Team::R2L, Window::getWindow().loadTexture(config->getName()),
                       config->getFrameWidth(), config->getFrameHeight(), config->getNumFrames());
        player->setFPS(14);

        m_playersUnused.push_back(player);
    }
}

void StartMenu::registerInputs(const std::vector<Input*> inputs) {
    // call base function
    Menu::registerInputs(inputs);

    // Check, if not too much inputs are there.
    uint64_t maxPlayers = m_gameconfig->getPlayers().size();
    if (inputs.size() > maxPlayers) {
        throw std::runtime_error("Too Many Inputs!");
    }

    // Map all teams to the given inputs
    Team team = Team::R2L;
    for (Input* i : inputs) {
        // get player and save it to m_playersUsed
        Player* player = m_playersUnused.back();
        m_playersUnused.pop_back();
        m_playersUsed.push_back(player);

        // register input
        player->registerInput(i);
        player->setTeam(team);

        // toggle teams
        if (team == Team::R2L) {
            team = Team::L2R;
        } else {
            team = Team::R2L;
        }
    }

    // Update divider height
    updateDividerHeight();
    updateStartEntry();
}

void StartMenu::updateDividerHeight() {
    int height = static_cast<int>(m_playersUsed.size()) * 60 + 100;
    m_divider->setH(height);
}

void StartMenu::setupUI() {
    int w = Window::getWindow().w();
    int h = Window::getWindow().h();

    // one tenths of padding here.
    int menuX = w / 10;
    int menuY = h / 10;

    // create all labels
    m_lblHeader = new Label("std_12px", "CaptureTheBanana++", Vector2dT(menuX, menuY), 2);
    m_lblHeader->setColor(Color(255, 0, 255));

    m_lblStart = new Label("std_12px", "Start", Vector2dT(menuX, menuY += 75), 2);
    m_menuLabels.emplace_back(m_lblStart, StartMenuAction::Start);

    m_menuLabels.emplace_back(new Label("std_12px", "Highscores", Vector2dT(menuX, menuY += 30), 2),
                              StartMenuAction::Highscores);
    // Just add the enable/disable keyboard only, if there are other controls and not too much.
    uint64_t maxPlayers = m_gameconfig->getPlayers().size();
    uint64_t inputs = Window::getInputManager().getInputs().size();
    if (inputs > 1 && inputs < maxPlayers) {
        std::string kb;
        if (Window::getInputManager().hasKeyboard()) {
            kb = "Disable keyboard";
        } else {
            kb = "Enable keyboard";
        }
        m_lblKeyboard = new Label("std_12px", kb, Vector2dT(menuX, menuY += 30), 2);
        m_menuLabels.emplace_back(m_lblKeyboard, StartMenuAction::Keyboard);
    } else {
        m_lblKeyboard = nullptr;
    }
    m_menuLabels.emplace_back(new Label("std_12px", "Edit game", Vector2dT(menuX, menuY += 30), 2),
                              StartMenuAction::EditGame);
    m_menuLabels.emplace_back(new Label("std_12px", "Credits", Vector2dT(menuX, menuY += 30), 2),
                              StartMenuAction::Credits);
    m_menuLabels.emplace_back(new Label("std_12px", "Quit", Vector2dT(menuX, menuY += 30), 2),
                              StartMenuAction::Quit);

    // --team1 (200px)--|--team2 (200px)-- <- menuX ->|
    // the divider is 5 px wide.
    menuY = h / 10;
    int teamX = w - menuX - 405;
    m_lblTeamR2L = new Label("std_12px", "L2R", Vector2dT(teamX + 100, menuY), 2);
    m_lblTeamR2L->setAlignment(LabelAlignment::Center);
    m_lblTeamL2R = new Label("std_12px", "R2L", Vector2dT(teamX + 305, menuY), 2);
    m_lblTeamL2R->setAlignment(LabelAlignment::Center);

    // Add divider. The right hight is set later
    SDL_Rect r = {teamX + 200, menuY - 25, 5, 200};
    m_divider = new Rect(r);
}

void StartMenu::updateBlink() {
    Uint32 ticks = SDL_GetTicks();
    float time = static_cast<float>(ticks - m_lastRenderTicks);

    // Check, if we have reached an update
    if (time > static_cast<float>(m_frameTimeout)) {
        m_menuLabels[m_selected].first->toggleShow();

        // Save current tick count
        m_lastRenderTicks = ticks;
    }
}

// Set the blink speed to the selected entry.
void StartMenu::setFPS(const int fps) {
    m_frameTimeout = static_cast<Uint32>(1000.0 / fps);
}

void StartMenu::render() {
    // render every label and the players
    renderBackground();
    updateBlink();

    m_divider->render();

    m_lblHeader->render();
    for (auto& lbl : m_menuLabels) {
        lbl.first->render();
    }
    m_lblTeamR2L->render();
    m_lblTeamL2R->render();

    renderPlayers();
}

void StartMenu::renderPlayers() {
    int w = Window::getWindow().w();
    int h = Window::getWindow().h();

    int xBase = (w * 9) / 10 - 405;
    int y = h / 10 + 50;
    // iterate over all players
    for (auto& player : m_playersUsed) {
        if (player->getTeam() == Team::R2L) {
            Vector2dT p(xBase + 305 - 24, y);
            player->renderStatic(p);
        } else {
            Vector2dT p(xBase + 100 - 24, y);
            player->renderStatic(p);
        }
        y += 60;
    }
}

void StartMenu::handleInput(InputType type, const Input* input, bool state, float /*angle*/) {
    // Just listen to key presses
    if (!state) {
        return;
    }

    switch (type) {
        case InputType::INPUT_SELECT:
            select();
            break;

        case InputType::INPUT_UP:
            // Special case: Do not go to 0 (start) if it is disabled
            if (m_selected > 1 || (m_startEnabled && m_selected > 0)) {
                m_menuLabels[m_selected].first->setShow(true);
                m_selected--;
            }
            break;

        case InputType::INPUT_DOWN:
            if (m_selected < m_menuLabels.size() - 1) {
                m_menuLabels[m_selected].first->setShow(true);
                m_selected++;
            }
            break;

        case InputType::INPUT_TEAM_1:  // join L2R
            // find the right player
            for (auto& player : m_playersUsed) {
                if (player->getInput() == input && player->getTeam() == Team::R2L) {
                    // found!
                    player->setTeam(Team::L2R);
                }
            }
            updateStartEntry();
            break;

        case InputType::INPUT_TEAM_2:  // join R2L
            // find the right player
            for (auto& player : m_playersUsed) {
                if (player->getInput() == input && player->getTeam() == Team::L2R) {
                    // found!
                    player->setTeam(Team::R2L);
                }
            }
            updateStartEntry();
            break;

        default:
            break;
    }
}

void StartMenu::select() {
    // Which entry is selected?
    switch (m_menuLabels[m_selected].second) {
        case StartMenuAction::Start:
            m_freePlayersUsed = false;
            Window::getEngine().getGame()->startGame(m_playersUsed);
            Window::getWindow().closeCurrentMenu();
            break;

        case StartMenuAction::Highscores: {
            HighscoreMenu* menu = new HighscoreMenu();
            Window::getWindow().addMenu(menu);
        } break;

        case StartMenuAction::Keyboard:
            if (m_lblKeyboard) {
                toggleKeyboard();
            }
            break;

        case StartMenuAction::EditGame: {
            Window::getWindow().quit();

            std::string editorPath{};
            std::vector<std::string> paths = {"ctb-editor" /*release*/,
                                              "../editor/ctb-editor" /*build*/};

            // try find the editor executable
            bool found = false;
            for (auto& item : paths) {
                if (boost::filesystem::exists(item)) {
                    editorPath = boost::filesystem::canonical(item).string();
                    found = true;
                    break;
                }
            }

            if (found) {
                boost::filesystem::path absoluteGameXml =
                    boost::filesystem::canonical(Window::getEngine().getGameFile());

                // execute it!
                execl(editorPath.c_str(), "ctb-editor", absoluteGameXml.string().c_str(), nullptr);
                throw std::runtime_error("Editor could not be started! (Editor path: " +
                                         editorPath);
            }
            break;
        }
        case StartMenuAction::Credits:
            Window::getWindow().addMenu(new CreditsMenu());
            break;

        case StartMenuAction::Quit:
            Window::getWindow().quit();
            break;
    }
}

void StartMenu::updateStartEntry() {
    // is a team empty?
    bool r2l_empty = true;
    bool l2r_empty = true;

    // check each player.
    for (auto& player : m_playersUsed) {
        if (player->getTeam() == Team::R2L) {
            r2l_empty = false;
        } else {
            l2r_empty = false;
        }
    }

    // One team is empty, so disable start.
    // But only, if the -d argument was not given...
    if ((r2l_empty || l2r_empty) && !Window::isDebug()) {
        Color c(128, 128, 128);
        m_lblStart->setColor(c);
        m_lblStart->setShow(true);
        m_startEnabled = false;
        if (m_selected == 0) {
            m_selected = 1;
        }
    } else {
        Color c(255, 255, 255);
        m_lblStart->setColor(c);
        m_lblStart->setShow(true);
        m_startEnabled = true;
    }
}

void StartMenu::toggleKeyboard() {
    if (Window::getInputManager().hasKeyboard()) {
        // remove the keyboard
        Keyboard* k = Window::getInputManager().getKeyboard();

        // Remove from player from used players
        auto it = std::find_if(m_playersUsed.begin(), m_playersUsed.end(),
                               [&k](Player* player) { return player->getInput() == k; });

        if (it != m_playersUsed.end()) {
            (*it)->deregisterInput();
            m_playersUnused.push_back(*it);
            m_playersUsed.erase(it);
        }

        // Deregister and remove inputhandler
        k->deregister_handler(m_inputHandlerIds[k]);
        m_inputHandlerIds.erase(k);

        // Finally delete the keyboard.
        Window::getInputManager().removeKeyboard();
        m_lblKeyboard->setText("Enable keyboard");
    } else {  // Add it.
        Window::getInputManager().addKeyboard();
        Keyboard* k = Window::getInputManager().getKeyboard();

        // Register the keyboard input
        registerInput(k);

        // use player
        Player* player = m_playersUnused.back();
        m_playersUnused.pop_back();
        player->setTeam(Team::L2R);
        player->registerInput(k);
        m_playersUsed.push_back(player);

        m_lblKeyboard->setText("Disable keyboard");
    }
    updateDividerHeight();
    updateStartEntry();
}

StartMenu::~StartMenu() {
    delete m_lblHeader;
    for (auto& lbl : m_menuLabels) {
        delete lbl.first;
    }
    delete m_lblTeamR2L;
    delete m_lblTeamL2R;

    delete m_divider;

    // Delete unused players
    for (auto& player : m_playersUnused) {
        delete player;
    }

    if (m_freePlayersUsed) {
        for (auto& player : m_playersUsed) {
            delete player;
        }
    }
}

}  // namespace engine
}  // namespace ctb
