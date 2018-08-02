// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_INPUT_INPUT_HPP
#define ENGINE_INPUT_INPUT_HPP
#include <forward_list>
#include <functional>
#include <map>
#include <string>

#include <SDL.h>

#include "engine/Object.hpp"

namespace ctb {
namespace engine {

/// enum for all occuring inputs, generically from Keyboard and mouse or Controller
enum class InputType {
    INPUT_LEFT,      // game: move left, menu: move left
    INPUT_RIGHT,     // game: move right, menu: move right
    INPUT_AIM_VERT,  // game: aim
    INPUT_SHOOT,     // game: shoot
    INPUT_JUMP,      // game: jump
    INPUT_HEAL,
    INPUT_TAUNT,
    INPUT_DROP_WEAPON,
    INPUT_PAUSE,   // game: enter pause-menu, pause-menu: reenter game
    INPUT_SELECT,  // menu: push one next menu on stack
    INPUT_RETURN,  // menu: pop one menu from stack, showing the one beneath
    INPUT_UP,      // menu: move up
    INPUT_DOWN,    // menu: move down
    INPUT_TEAM_1,  // menu: join TEAM_L2R
    INPUT_TEAM_2   // menu: join TEAM_R2L
};

/// enum used in getType() to downcast savely from Input
enum class InputDeviceType { Keyboard, Controller };

/// forward declaration for following typedef
class Input;

/// typedef for a function wrapper for functions of type void foo(InputType, Input*, bool, float)
/// that can be called, if an input accours. The first argument is the input type, e.g. INPUT_SHOOT.
/// The second argument is the input, that fired this event. THe state is whether
/// the key was pressed (true) or released (false). For the aiming event,
/// the current angle is provided between -90 and 90 degree. For other event types
/// this argument is invalid.
typedef std::function<void(const InputType, Input*, const bool, const float)> InputHandler;

/// Interface for every input device
class Input : public Object {
   public:
    /// To be called by subclasses (Keyboard and Controller) to provide full functionality
    Input();

    /***
     * Update the keystates for the input
     *
     * @param keyStates		A representation of the keyboard to check if a key is pressed or not
     */
    virtual void pollInput(const Uint8* keyStates) = 0;

    /***
     * Handle SDl events for this input
     *
     * @param event		Used to access more specific information about the event
     */
    virtual void handleSdlEvent(const SDL_Event& event) = 0;

    /***
     * Registers the given handler for this input
     *
     * @param handler	The handler to call if the input changes
     *
     * @returns 	A unique id to remove this handler later
     */
    uint64_t register_handler(InputHandler& handler);

    /***
     * Reregisters the handler by the id
     *
     * @param id	the mentioned id
     */
    void deregister_handler(uint64_t id);

    /// Get the device type
    /// \return the device type.
    virtual InputDeviceType getType() = 0;

    /// Destructor
    ~Input() override = default;

   protected:
    /// Call all registered handlers for the event
    /// \param type The event type
    /// \param state the button state
    /// \param angle Optional, may be provided with an aim event. Otherwise this argument
    ///              is invalid.
    void call_handlers(InputType type, bool state, float angle = 0.0);

   private:
    /// Static counter for input handler ids.
    static uint64_t idCounter;

    /// Saves the handlers associated with the handler ids
    std::map<uint64_t, InputHandler> m_handlers;

    /// Saves the last states for the input device
    std::map<InputType, bool> m_lastStates;
};

}  // namespace engine
}  // namespace ctb

#endif
