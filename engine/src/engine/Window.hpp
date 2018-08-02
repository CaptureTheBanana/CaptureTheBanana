// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_WINDOW_HPP
#define ENGINE_WINDOW_HPP

#include <queue>
#include <stack>
#include <string>
#include <vector>

#include <SDL.h>

#include "engine/Object.hpp"

namespace ctb {
namespace engine {

class Engine;
class Menu;
class InputManager;
class Input;

/// Window arguments
struct WindowArguments {
    /// Enable debug mode
    bool debug{false};
    // Show more debug information
    bool verbose{false};
    /// Enable sound
    bool sound{true};
    /// Game file path
    std::string path{};
};

class Window : public Object {
   public:
    static void run(const std::string& title, int width, int height, const WindowArguments& args);

    Window() = delete;
    Window(Window const&) = delete;
    void operator=(Window const&) = delete;

    static Window& getWindow();

    static Engine& getEngine();

    static InputManager& getInputManager();

    static bool isDebug();

    /// Quits the game
    void quit() { m_quit = true; }

    /// Push menu to the current menu
    void addMenu(Menu* menu);

    /// Closes the current menu
    void closeCurrentMenu();

    /// Returns the current open menu
    Menu* getCurrentMenu();

    /// Gets the current SDL renderer
    SDL_Renderer* renderer() const { return m_renderer; }

    /// Returns the current width of the window
    int w() const { return m_width; }

    /// Returns the current height of the window
    int h() const { return m_height; }

    /// \brief Returns a SDL_Texture from the given image.
    ///
    /// \param path Path to the image
    /// \param colorKeyRed Color key red
    /// \param colorKeyGreen Color key green
    /// \param colorKeyBlue Color key blue
    /// \return SDL_Texture SDL texture
    SDL_Texture* loadTexture(const std::string& path,
                             const Uint8 colorKeyRed = 255,
                             const Uint8 colorKeyGreen = 0,
                             const Uint8 colorKeyBlue = 255);

    virtual ~Window();

   private:
    static Window* instance;

    static bool DEBUG;
    static bool VERBOSE;

    /***
     * Creates the main window with given \ref title, width \ref w and height \ref h
     *
     * @param title		Title of the window
     * @param gamefile  XML definition of game attributes
     * @param w			Width
     * @param h			Height
     * @param sound     Sound enabled
     */
    Window(const std::string& title, const std::string& gamefile, const int w, const int h);

    void init(const bool sound);

    void run();

    /// Initializes all needed SDL resources
    void initSDL(const std::string& title);

    /// Quits SDL and frees all resources
    void quitSDL();

    bool m_quit{false};

    /// Game engine
    Engine* m_engine{nullptr};

    /// SDL main window struct
    SDL_Window* m_window{nullptr};

    /// SDL renderer struct
    SDL_Renderer* m_renderer{nullptr};

    /// Window width
    int m_width;

    /// Window height
    int m_height;

    /// Stack of Menu objects
    std::stack<Menu*> m_menus;

    /// Add menus after the main loop emits all keys to the other menus
    /// to prevent the new created menu to listen to the old keys
    std::queue<Menu*> m_menusToAdd;

    InputManager* m_inputManager;
};

}  // namespace engine
}  // namespace ctb

#endif  // ENGINE_WINDOW_HPP
