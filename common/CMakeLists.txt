cmake_minimum_required(VERSION 3.1)

set(COMMON_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/ConsoleColor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/Exceptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/Utils.cpp
)

set(COMMON_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/ConsoleColor.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/Exceptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/Utils.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/Vector2d.hpp
)

# Build library
add_library(common-static STATIC ${COMMON_SOURCES} ${COMMON_HEADERS})
target_link_libraries(common-static ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
target_include_directories(
    common-static PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
