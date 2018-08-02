// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_INPUT_CONTROLLER_HPP
#define ENGINE_INPUT_CONTROLLER_HPP

#include <SDL.h>

#include "engine/input/Input.hpp"

namespace ctb {
namespace engine {

/***
 * A class to handle input from a controller
 */
class Controller : public Input {
   public:
    /***
     * Creates an instance of Controller  wrapping an \ref SDL_GameController
     *
     * @param controller		Pointer to the wrapped SDL_GameController
     */
    explicit Controller(SDL_GameController* controller);

    /***
     * Called with an \ref SDL_Event and delegates to either handleAxisMotion() or handleButton()
     *
     * @param event	The occuring event, either an SDL_ControllerAxisEvent or an
     * SDL_ControllerButtonEvent
     */
    void handleSdlEvent(const SDL_Event& event) override;

    /***
     *  Name for rendering in the start menu
     *
     *  @return	An InputDeviceType to cast safely from Input (to Controller)
     */
    InputDeviceType getType() override { return InputDeviceType::Controller; }

    /// Destructor
    ~Controller() override;

    /***
     * Used as a tick for aiming, no polling needed in Controller, so the param remains unnused
     *
     * @param Uint8*	Keyboard repreesentation, ignored in this class, inherited from Input
     */
    void pollInput(const Uint8*) override;

   private:
    /// An SDL_GameController wrapped in this class
    SDL_GameController* m_controller;

    /// SDL-given id
    SDL_JoystickID m_id;

    /***
     * Handles AxisMotion events, including triggers
     *
     * @param event	Occuring event to get details
     */
    void handleAxisMotion(const SDL_ControllerAxisEvent& event);

    /***
     * Handles Button events, including pressed sticks
     *
     * @param event	Occuring event to get details
     */
    void handleButton(const SDL_ControllerButtonEvent& event);

    /// Representing the current angle for aiming.
    float m_angle;

    /// The last saved angle. Used for checking, if the angle was changed.
    float m_lastAngle;

    /// Saves, if the left stick is outside of the dead zone, so the player is aiming.
    bool m_aiming;

    /// Representing the current value of the stick for aiming.
    int m_valAim;

    /// Representing the current left right value
    int m_lastLR;

    /// Representing the current up down value
    int m_lastUD;
};

}  // namespace engine
}  // namespace ctb

#endif
