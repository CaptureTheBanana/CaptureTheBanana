cmake_minimum_required(VERSION 3.1)

project(CAPTURE_THE_BANANA)

option(BUILD_EDITOR "Build with editor" ON)
option(BUILD_TESTS "Build with tests" ON)
option(BOX2D_USE_SHARED "Use the shared Box2D library on Linux; otherwise build static Box2D" ON)
option(ENABLE_SOUND "Build with SDL2 sound" ON)
option(ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(ENABLE_ASAN "Enable address sanitizer" OFF)
option(ENABLE_UBSAN "Enable undefined sanitizer" OFF)
option(ENABLE_SANITIZER_BLACKLIST "Enable sanitizer blacklist - must be created manually" OFF)

set(CTB_VERSION_MAJOR 0)
set(CTB_VERSION_MINOR 0)
set(CTB_VERSION_PATCH 1)

if(${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    message(FATAL_ERROR "In-source builds are not allowed.")
endif()

if(ENABLE_ASAN OR ENABLE_UBSAN)
    set(CMAKE_BUILD_TYPE DEBUG)
elseif (NOT CMAKE_BUILD_TYPE)
    # No build type selected
    set(CMAKE_BUILD_TYPE DEBUG)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wpedantic -Wnon-virtual-dtor -Woverloaded-virtual -Wsign-conversion -Wconversion -Wold-style-cast")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG")
    if (ENABLE_WARNINGS_AS_ERRORS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
    endif()
elseif(MSVC)
    if(CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Visual Studio configuration types" FORCE)
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /MP")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /O2 /DNDEBUG")
    add_definitions(-DNOMINMAX=1)
else()
    message(FATAL_ERROR "Unknown compiler")
endif()

# Enable sanitizer
if(ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize-address-use-after-scope")
endif()
if(ENABLE_UBSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -fno-omit-frame-pointer")
endif()
if(ENABLE_SANITIZER_BLACKLIST)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize-blacklist=SanitizerBlacklist")
endif()

# Enable C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CMake module path
set(CMAKE_MODULE_PATH ${CAPTURE_THE_BANANA_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})

###########################################################
# Add dependencies
#

# SDL2 and SDL2_image
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
find_package(SDL2_IMAGE REQUIRED)
include_directories(${SDL2_IMG_INCLUDE_DIR})

# SDL2_mixer
if(ENABLE_SOUND)
    add_definitions(-DENABLE_SOUND=1)
    find_package(SDL2_MIXER REQUIRED)
    include_directories(${SDL2_MIXER_INCLUDE_DIR})
endif()

# Boost
if(MSVC)
    #set(Boost_DEBUG ON)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
    set(Boost_USE_MULTITHREADED ON)
endif()

find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Box2D
if (APPLE OR WIN32 OR NOT BOX2D_USE_SHARED)
    # TODO: Move (merge) Box2D+Box2D-cmake to our organisation and add a git submodule

    # Brew's Box2D is outdated
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/Box2D/Box2D)
else()
    find_package(Box2D REQUIRED)
    include_directories(${Box2D_SOURCE_DIR})
endif()

# Include third-party directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

# Enable GSL exceptions
add_definitions(-Dgsl_CONFIG_CONTRACT_VIOLATION_THROWS)

###########################################################
# Configuration files
#

set(CMAKE_CONFIG_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)

# Version
set(CTB_VERSION_NAME CACHE STRING "Version information")
if (NOT CTB_VERSION_NAME)
    set(CTB_VERSION_NAME "-dev")
endif()
configure_file(cmake/CtbVersion.in ${CMAKE_CONFIG_DIR}/CtbVersion.hpp @ONLY)

###########################################################
# Add projects
#

# common
add_subdirectory(common)

# parser
add_subdirectory(parser)

# engine
add_subdirectory(engine)

# CaptureTheBanana
add_subdirectory(ctb)

# editor
add_subdirectory(editor)

###########################################################
# Build tests
#

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

###########################################################
# Build package
#

# TODO: Optionally include editor and Qt5 dependency and/or build another package

set(CPACK_PACKAGE_NAME "CaptureTheBanana++")
set(CPACK_PACKAGE_FILE_NAME "CaptureTheBanana_${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_PACKAGE_VENDOR "The CaptureTheBanana++ contributors")
set(CPACK_PACKAGE_CONTACT "The CaptureTheBanana++ core maintainers")
set(CTP_PACKAGE_DESCRIPTION "CaptureTheBanana++ is a multiplayer PvP game, focused on fast pace combat to allow to carry a flag to the respective endscreen.")
set(CPACK_PACKAGE_VERSION_MAJOR "${CTB_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${CTB_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${CTB_VERSION_PATCH}")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)

set(CTB_SHARE_NAME "share/capture-the-banana")

if(APPLE)
    set(CPACK_GENERATOR DragNDrop)
elseif(CPACK_GENERATOR STREQUAL "DEB")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "${CTP_PACKAGE_DESCRIPTION}")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-filesystem (>=1.58.0),libboost-system (>=1.58.0),libbox2d (>=2.3.1),libsdl2,libsdl2-image,libsdl2-mixer")
elseif(CPACK_GENERATOR STREQUAL "RPM")
    set(CPACK_RPM_PACKAGE_DESCRIPTION "${CTP_PACKAGE_DESCRIPTION}")
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")
    set(CPACK_RPM_PACKAGE_REQUIRES "boost>=1.58,Box2D>=2.3.1,SDL2,SDL2_image,SDL2_mixer")
else()
    set(CTB_SHARE_NAME ".")
    if (UNIX)
        set(CPACK_GENERATOR TGZ)
    else()
        set(CPACK_GENERATOR ZIP)
    endif()
endif()

install(FILES ${CMAKE_SOURCE_DIR}/CONTRIBUTORS.md DESTINATION "${CTB_SHARE_NAME}" COMPONENT CTB)
install(FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION "${CTB_SHARE_NAME}" COMPONENT CTB)
install(FILES ${CMAKE_SOURCE_DIR}/THIRDPARTY.md DESTINATION "${CTB_SHARE_NAME}" COMPONENT CTB)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/res DESTINATION "${CTB_SHARE_NAME}" COMPONENT CTB)

include(CPack)
