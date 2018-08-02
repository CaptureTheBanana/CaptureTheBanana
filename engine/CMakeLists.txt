cmake_minimum_required(VERSION 3.1)

set(ENGINE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/Engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/Window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/audio/Music.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/audio/SoundDummy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/audio/SoundEffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/audio/SoundManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/core/Camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/core/Game.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/core/GC.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/core/Level.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/ActingRenderable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/Background.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/LayerRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/PhysicalRenderable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/Rect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/TextureBasedRenderable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/TilesetRenderable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/gui/Font.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/gui/Label.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/gui/Statusbar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/input/Controller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/input/Input.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/input/InputManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/input/Keyboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/menu/CreditsMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/menu/EndMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/menu/HighscoreMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/menu/Menu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/menu/NewHighscoreMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/menu/PauseMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/menu/StartMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/misc/Highscores.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/physics/ActingKinematics.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/physics/Kinematics.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/physics/LevelContactListener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/physics/LevelWorld.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/physics/PhysicalObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/physics/PhysicalTileSet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scene/Player.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scene/Bot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scene/Door.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scene/Fist.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scene/Flag.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scene/Gun.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scene/Projectile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scene/Ufo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scene/Zombie.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/util/Random.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/util/SdlDriver.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/Engine.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/Object.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/Window.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/audio/Music.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/audio/SoundEffect.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/audio/SoundManager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/core/Camera.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/core/Game.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/core/GC.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/core/Level.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/ActingRenderable.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/Background.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/LayerRenderer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/PhysicalRenderable.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/Rect.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/SDLRenderable.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/TextureBasedRenderable.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/TilesetRenderable.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/gui/Font.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/gui/Label.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/gui/Statusbar.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/input/Controller.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/input/Input.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/input/InputManager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/input/Keyboard.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/menu/CreditsMenu.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/menu/CreditsMenu.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/menu/EndMenu.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/menu/HighscoreMenu.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/menu/Menu.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/menu/NewHighscoreMenu.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/menu/PauseMenu.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/menu/StartMenu.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/misc/Highscores.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/physics/ActingKinematics.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/physics/Kinematics.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/physics/LevelContactListener.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/physics/LevelWorld.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/physics/PhysicalObject.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/physics/PhysicalTileSet.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scene/Player.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scene/Bot.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scene/Door.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scene/Fist.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scene/Flag.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scene/Gun.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scene/Projectile.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scene/Ufo.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scene/Weapon.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scene/Zombie.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/util/Color.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/util/Exceptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/util/Vector2d.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/util/Random.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/util/SdlDriver.hpp
)

# Build engine library
add_library(engine-static STATIC ${ENGINE_SOURCES})
target_link_libraries(engine-static ${SDL2_LIBRARY} ${SDL2_IMG_LIBRARY})
if(ENABLE_SOUND)
    target_link_libraries(engine-static ${SDL2_MIXER_LIBRARY})
endif()
target_link_libraries(engine-static ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
target_link_libraries(engine-static Box2D)
if(NOT MSVC)
    target_link_libraries(engine-static m)
endif()
target_link_libraries(engine-static common-static parser-static)
target_include_directories(engine-static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
