// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_MENU_MENU_HPP
#define ENGINE_MENU_MENU_HPP

#include <map>
#include <vector>

#include <SDL.h>

#include "engine/graphics/SDLRenderable.hpp"
#include "engine/input/Input.hpp"

namespace ctb {
namespace engine {

/// \brief The abstract base class for all menus. Provides a base handling of inputs.
class Menu : public SDLRenderable {
   public:
    Menu() = default;

    void render() override = 0;

    /// Register the input input.
    /// \param Input* input the input to register
    virtual void registerInput(Input* input);

    /// Registers multiple inputs. Calls registerInput for every one.
    /// \param std::vector<Input*> List of inputs to register.
    virtual void registerInputs(std::vector<Input*> inputs);

    /// Deregister all handlers.
    virtual void deregisterAllHandlers();

    /// Can be used for raw keyboad input for typing text in menus.
    virtual void handleSdlEvent(const SDL_Event& /*event*/) {}

    /// Destruktor
    virtual ~Menu();

   protected:
    /// Has to be implemented by every menu.
    virtual void handleInput(InputType type, const Input* input, bool state, float angle) = 0;

    /// Renders the black transparent background for the menus.
    void renderBackground();

    /// Map the input to input handler ids.
    std::map<Input*, uint64_t> m_inputHandlerIds;

   private:
    /// This function checks, if the menu is the current menu. If so, the
    /// handleInput method gets called.
    void _handleInput(InputType type, const Input* input, bool state, float angle);
};

}  // namespace engine
}  // namespace ctb

#endif
