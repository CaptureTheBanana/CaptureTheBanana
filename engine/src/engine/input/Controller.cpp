// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/input/Controller.hpp"

namespace ctb {
namespace engine {

Controller::Controller(SDL_GameController* controller)
    : m_controller(controller),
      m_angle(0.0),
      m_lastAngle(0.0),
      m_aiming(false),
      m_valAim(0),
      m_lastLR(0),
      m_lastUD(0) {
    m_id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));
}

void Controller::handleSdlEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_CONTROLLERAXISMOTION:
            if (event.caxis.which == m_id) {
                handleAxisMotion(event.caxis);
            }
            break;
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
            if (event.cbutton.which == m_id) {
                handleButton(event.cbutton);
            }
            break;
    }
}

void Controller::handleAxisMotion(const SDL_ControllerAxisEvent& event) {
    int value = event.value;

    // check if the value is outside of the dead zone (inner center)
    if (value > 20000 || value < -20000) {
        switch (event.axis) {
            // go left and right
            case SDL_CONTROLLER_AXIS_LEFTX:

                // go left
                if (value < 0 && m_lastLR >= 0) {
                    m_lastLR = value;
                    call_handlers(InputType::INPUT_LEFT, true);
                    call_handlers(InputType::INPUT_TEAM_1, true);
                }
                // go right
                if (value > 0 && m_lastLR <= 0) {
                    m_lastLR = value;
                    call_handlers(InputType::INPUT_RIGHT, true);
                    call_handlers(InputType::INPUT_TEAM_2, true);
                }
                break;

            // menu selection (up and down)
            case SDL_CONTROLLER_AXIS_LEFTY:

                // up
                if (value < 0 && m_lastUD >= 0) {
                    m_lastUD = value;
                    call_handlers(InputType::INPUT_UP, true);
                }

                // down
                if (value > 0 && m_lastUD <= 0) {
                    m_lastUD = value;
                    call_handlers(InputType::INPUT_DOWN, true);
                }
                break;

            // right stick moved
            case SDL_CONTROLLER_AXIS_RIGHTY:
                // Set aiming to true, because the value is outside of the dead zone
                m_aiming = true;
                m_valAim = value;
        }
    } else {  // dead zone
        // Set axis value to 0
        if (event.axis == SDL_CONTROLLER_AXIS_RIGHTY) {
            m_valAim = 0;
        }

        // not moving the player
        if (event.axis == SDL_CONTROLLER_AXIS_LEFTX && m_lastLR) {
            m_lastLR = 0;
            call_handlers(InputType::INPUT_RIGHT, false);
            call_handlers(InputType::INPUT_LEFT, false);
            call_handlers(InputType::INPUT_TEAM_1, false);
            call_handlers(InputType::INPUT_TEAM_2, false);
        }

        // not moving up or down (in menu)
        if (event.axis == SDL_CONTROLLER_AXIS_LEFTY && m_lastUD) {
            m_lastUD = 0;
            call_handlers(InputType::INPUT_UP, false);
            call_handlers(InputType::INPUT_DOWN, false);
        }
    }

    // LT and RT both for shooting
    if (event.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT ||
        event.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) {
        // trigger released
        if (value < 1000) {
            call_handlers(InputType::INPUT_SHOOT, false);
        } else {
            // trigger pressed
            call_handlers(InputType::INPUT_SHOOT, true);
        }
    }
}

void Controller::pollInput(const Uint8* /*k*/) {
    // Update m_angle, if we are aiming.
    if (!m_aiming) {
        return;
    }
    float scale_dy = 0.02f;

    // Amout of value, that we should add (or substract) from m_angle
    float dy = scale_dy * static_cast<float>(m_valAim / 100 * -1);

    m_angle += dy;
    // check for bounds
    if (m_angle > 90.0) {
        m_angle = 90.0;
    } else if (m_angle < -90.0) {
        m_angle = -90.0;
    }

    // Call handlers, if the angle was changes.
    if (m_angle != m_lastAngle) {
        call_handlers(InputType::INPUT_AIM_VERT, true, m_angle);

        // Save new angle
        m_lastAngle = m_angle;
    }
}

void Controller::handleButton(const SDL_ControllerButtonEvent& event) {
    int pressed = event.state;

    switch (event.button) {
        // jump
        case SDL_CONTROLLER_BUTTON_A:
            call_handlers(InputType::INPUT_JUMP, pressed);

            // in menu: select button
            call_handlers(InputType::INPUT_SELECT, pressed);
            break;

        // in menu: exit from current (sub-)menu
        case SDL_CONTROLLER_BUTTON_B:
            call_handlers(InputType::INPUT_RETURN, pressed);

            // call_handlers(InputType::INPUT_DUCK, pressed);
            break;

        // heal
        case SDL_CONTROLLER_BUTTON_Y:
            call_handlers(InputType::INPUT_HEAL, pressed);
            break;

        // taunt
        case SDL_CONTROLLER_BUTTON_X:
            // taunt
            call_handlers(InputType::INPUT_TAUNT, pressed);
            break;

        case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
            call_handlers(InputType::INPUT_DROP_WEAPON, pressed);
            break;

        // in menu: join team 1
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
            call_handlers(InputType::INPUT_TEAM_1, pressed);
            break;

        // in menu: join team 2
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
            call_handlers(InputType::INPUT_TEAM_2, pressed);
            break;

        // in menu: exit from current (sub-)menu
        case SDL_CONTROLLER_BUTTON_BACK:
            call_handlers(InputType::INPUT_RETURN, pressed);
            break;

        // call or quit pause-menu
        case SDL_CONTROLLER_BUTTON_START:
            call_handlers(InputType::INPUT_PAUSE, pressed);
            break;
    }
}

Controller::~Controller() {
    SDL_GameControllerClose(m_controller);
}

}  // namespace engine
}  // namespace ctb
