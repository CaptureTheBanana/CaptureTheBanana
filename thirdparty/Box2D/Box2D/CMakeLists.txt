cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

project(Box2D)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RELEASE)
endif()

#option(BOX2D_INSTALL "Install Box2D library, includes, and CMake scripts" OFF)
#option(BOX2D_BUILD_SHARED "Build Box2D shared library" ON)
set(BOX2D_INSTALL 0)
set(BOX2D_BUILD_SHARED 0) # Build static

# Our current version is master
set(BOX2D_VERSION 2.3.2)
set(LIB_INSTALL_DIR lib${LIB_SUFFIX})

if (CMAKE_VERSION VERSION_LESS "3.1")
    if(NOT MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif()
else()
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# The Box2D library.
add_subdirectory(Box2D)
