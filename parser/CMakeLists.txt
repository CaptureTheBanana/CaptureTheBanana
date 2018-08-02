cmake_minimum_required(VERSION 3.1)

# Setup sources
set(GAME_PARSER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/DynamicTilestore.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/GameParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/LevelParser.cpp
)

set(GAME_PARSER_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/BackgroundConfig.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/BotConfig.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/DynamicTilestore.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/GameConfig.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/GameParser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/LevelConfig.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/LevelParser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/PlayerConfig.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/ProjectileConfig.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/TilesetConfig.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/WeaponConfig.hpp
)

# Build library
add_library(parser-static STATIC ${GAME_PARSER_SOURCES} ${GAME_PARSER_HEADERS})
target_link_libraries(parser-static ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
target_link_libraries(parser-static common-static)
if(NOT MSVC)
    target_link_libraries(parser-static m)
endif()
target_include_directories(
    parser-static PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
