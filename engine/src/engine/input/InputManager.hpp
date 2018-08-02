// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_KEYBOARDMANAGER_HPP
#define ENGINE_KEYBOARDMANAGER_HPP

#include <string>
#include <vector>

namespace ctb {
namespace parser {
class GameConfig;
}

namespace engine {

class Input;
class Keyboard;

/// Class that handles user input
class InputManager {
   public:
    InputManager(parser::GameConfig* config);
    virtual ~InputManager();

    bool update();

    /// Check, if the keyboard is in the inputs
    bool hasKeyboard() const { return m_keyboard != 0 && !m_addKeyboard; }

    /// Return the keyboard pointer. May be null!
    Keyboard* getKeyboard() const { return m_keyboard; }

    /// Returns a list of input devices
    std::vector<Input*>& getInputs() { return m_inputs; }

    /// Add a keyboard to the inputs
    void addKeyboard();

    /// Remove the keyboard from the inputs
    void removeKeyboard();

   private:
    /// Initialize all inputs found on execution
    void setupDefaultInputs(const std::string& controllerMapFile);

    /// A flag that indicates, if m_keyboard should be added to m_inputs after
    /// the iteration over m_inputs is done.
    bool m_addKeyboard;

    /// A Vector of all registered inputs
    std::vector<Input*> m_inputs;

    /// To control the keyboard in m_inputs
    Keyboard* m_keyboard;
};

}  // namespace engine
}  // namespace ctb

#endif  // ENGINE_KEYBOARDMANAGER_HPP
