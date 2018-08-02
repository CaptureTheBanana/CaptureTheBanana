// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <SDL.h>
#include <SDL_image.h>

#include "engine/Engine.hpp"
#include "engine/Window.hpp"
#include "engine/audio/SoundManager.hpp"
#include "engine/core/GC.hpp"
#include "engine/gui/Font.hpp"
#include "engine/input/Input.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/menu/Menu.hpp"
#include "engine/misc/Highscores.hpp"
#include "engine/util/Exceptions.hpp"
#include "engine/util/SdlDriver.hpp"

namespace ctb {
namespace engine {

bool Window::DEBUG = false;
bool Window::VERBOSE = false;
Window* Window::instance = nullptr;

void Window::run(const std::string& title, int width, int height, const WindowArguments& args) {
    Window::DEBUG = args.debug;
    Window::VERBOSE = args.verbose;
    instance = new Window(title, args.path, width, height);
    instance->init(args.sound);
    instance->run();
    delete instance;
}

Window::Window(const std::string& title, const std::string& gamefile, const int w, const int h)
    : m_width(w), m_height(h) {
    // Initialize SDL
    initSDL(title);

    m_engine = new Engine(gamefile);
    m_inputManager = new InputManager(m_engine->getGameConfig());
}

Window& Window::getWindow() {
    if (instance == nullptr) {
        throw std::logic_error("You have to initialize the Window!");
    }
    return *instance;
}

Engine& Window::getEngine() {
    if (instance == nullptr) {
        throw std::logic_error("You have to initialize the Window!");
    }
    return *(instance->m_engine);
}

InputManager& Window::getInputManager() {
    if (instance == nullptr) {
        throw std::logic_error("You have to initialize the Window!");
    }
    return *(instance->m_inputManager);
}

bool Window::isDebug() {
    return Window::DEBUG;
}

void Window::init(const bool sound) {
    auto* config = m_engine->getGameConfig();

    // Init highscore system
    Highscores::init("./highscores");

    // Init sound system
    SoundManager::init(config, sound);

    // load all fonts in the folder
    Font::loadFonts(config->getFontsFolder());

    m_engine->restart();
}

void Window::run() {
    SDL_Rect background = {0, 0, m_width, m_height};

    // Start main loop and event handling
    while (!m_quit && m_renderer) {
        // add menus
        while (!m_menusToAdd.empty()) {
            m_menus.push(m_menusToAdd.front());
            m_menusToAdd.pop();
        }

        // Clear screen and render game
        SDL_RenderClear(m_renderer);

        // Draw white background
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(m_renderer, &background);

        if (m_inputManager->update()) {
            quit();
        }

        m_engine->update();

        if (!m_menus.empty()) {
            m_menus.top()->render();
        }

        // Update screen
        SDL_RenderPresent(m_renderer);

        GC::execute();
    }
}

void Window::addMenu(Menu* menu) {
    m_menusToAdd.push(menu);
}

void Window::closeCurrentMenu() {
    if (!m_menus.empty()) {
        Menu* menu = m_menus.top();
        m_menus.pop();
        GC::add(menu);
    }
}

Menu* Window::getCurrentMenu() {
    if (m_menus.empty()) {
        return nullptr;
    } else {
        return m_menus.top();
    }
}

SDL_Texture* Window::loadTexture(const std::string& path,
                                 const Uint8 colorKeyRed,
                                 const Uint8 colorKeyGreen,
                                 const Uint8 colorKeyBlue) {
    // Load image from disk
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr) {
        throw SdlException("Error while loading \"" + path + "\".", IMG_GetError());
    }

    auto defer = gsl::finally([&] { SDL_FreeSurface(surface); });

    // Map transparent color key
    SDL_SetColorKey(surface, SDL_TRUE,
                    SDL_MapRGB(surface->format, colorKeyRed, colorKeyGreen, colorKeyBlue));

    // Render SDL2 texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    if (texture == nullptr) {
        throw SdlException("Error while creating texture \"" + path + "\".", SDL_GetError());
    }
    return texture;
}

void Window::initSDL(const std::string& title) {
    // Initialize SDL
    Uint32 flags = SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER;
#ifdef ENABLE_SOUND
    flags |= SDL_INIT_AUDIO;
#endif
    if (SDL_Init(flags) != 0) {
        throw SdlException("SDL error during initialize.", SDL_GetError());
    }

    // Index of the rendering driver; -1 to initialize the first one supporting the requested flags.
    int renderDriverIndex = sdl_driver::getCurrentRenderBackend();

    if (VERBOSE) {
#ifndef NDEBUG
        std::cout << "Available rendering backends:\n";
        auto sdlDrivers = sdl_driver::getRenderBackends();
        for (auto& info : sdlDrivers) {
            std::cout << info.index << ": " << info.name << " (" << info.isSoftwareRenderer << ")"
                      << std::endl;
        }
#endif
        std::cout << "Selected rendering backend: "
                  << sdl_driver::getRenderBackendNameByIndex(renderDriverIndex) << std::endl;

#ifndef NDEBUG
        std::cout << "Available video drivers:\n";
        for (int i = 0; i < SDL_GetNumVideoDrivers(); ++i) {
            std::cout << SDL_GetVideoDriver(i) << std::endl;
        }
#endif
        std::cout << "Selected video driver: " << sdl_driver::getVideoDriverName() << std::endl;
    }

    // Generate SDL main window
    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                m_width, m_height, SDL_WINDOW_SHOWN);
    if (m_window == nullptr) {
        throw SdlException("SDL window could not be generated.", SDL_GetError());
    }

    // Create renderer for the SDL main window
    m_renderer = SDL_CreateRenderer(m_window, renderDriverIndex,
                                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_renderer == nullptr) {
        throw SdlException("SDL could not generate renderer.", SDL_GetError());
    }

    if (VERBOSE) {
        std::cout << "Current rendering backend: " << sdl_driver::getRenderBackendName(m_renderer)
                  << std::endl;
    }

    // Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
        throw SdlException("SDL_image could not initialize.", IMG_GetError());
    }

    if (!Window::isDebug()) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    // Render transparency
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
}

void Window::quitSDL() {
    // Destroy window and renderer
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }

    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    // Quit SDL and SDL_Image
    IMG_Quit();
    SDL_Quit();
}

Window::~Window() {
    // delete engine
    delete m_engine;

    // delete menus
    while (!m_menusToAdd.empty()) {
        m_menus.push(m_menusToAdd.front());
        m_menusToAdd.pop();
    }

    while (!m_menus.empty()) {
        auto* menu = m_menus.top();
        m_menus.pop();
        delete menu;
    }

    // free all input devices
    delete m_inputManager;

    // End sounds
    SoundManager::deleteSoundManager();

    // Quit SDL
    quitSDL();
}
}  // namespace engine
}  // namespace ctb
