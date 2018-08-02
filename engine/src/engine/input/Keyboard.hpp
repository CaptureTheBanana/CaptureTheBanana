// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef _ENGINE_INPUT_KEYBOARD_HPP
#define _ENGINE_INPUT_KEYBOARD_HPP

#include <map>
#include <vector>

#include "engine/input/Input.hpp"

namespace ctb {
namespace engine {

/***
 * A class to handle input from keyboard and mouse
 */
class Keyboard : public Input {
   public:
    /***
     * Creates a Keyboard with given \ref window_h
     */
    Keyboard();

    ~Keyboard() override = default;

    /***
     * Handles occuring mousemotion and -button events and calls corresponding
     *
     * @param event		the occuring (mouse-)event, needed to access more
     * 				specific information about the event
     */
    void handleSdlEvent(const SDL_Event& event) override;

    /***
     *  Name for rendering in the start menu
     *
     *  @return	An InputDeviceType to cast safely from Input (to Keyboard)
     */
    InputDeviceType getType() override { return InputDeviceType::Keyboard; }

    /***
     * Handles occuring mousemotion and -button events
     *
     * @param event		the occuring (mouse-)event, needed to access more
     * 				specific information about the event
     */
    void pollInput(const Uint8* keyStates) override;

   private:
    /// Map all key actions to input types.
    std::map<InputType, std::vector<unsigned int>> m_keyMap;
};

}  // namespace engine
}  // namespace ctb
#endif
