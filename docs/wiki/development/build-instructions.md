# Build instructions

Clone this repository and make an out-of-source build:

```
git clone https://github.com/CaptureTheBanana/CaptureTheBanana.git
cd CaptureTheBanana
mkdir build
cmake .. && make

ctb/CaptureTheBanana
```

If you find a bug -- keep it :butterfly: 

## Prerequisites

- C++14 compatible compiler such as `g++` 5.0 or `clang` 4.0 or above
- CMake 3.1 or above
- Boost library 1.57 or above
- Box2D (Linux only)
- SDL2 and SDL2_image
- clang-format (optional)
- Qt version >= v5.9 (optional, for editor)
- SDL Mixer >= 2 (optional, for sound)
- Doxygen (optional, for documentation generation)

## Windows build instructions

**Requirements:**

- Git and CMake
- Boost with filesystem and system development libraries
- SDL2, SDL2_image and SDL2_mixer with development libraries
- Qt5 (optional)
- Visual Studio 2017 or above

Define following environment variables or pass `-D...` to CMake:

- `BOOST_ROOT`: Path to boost root
- `BOOST_LIBRARYDIR`: Path to boost libraries
- `SDL2_ROOT`: Path to SDL2 root
- `SDL2_IMAGE_ROOT`: Path to SDL2_image root
- `SDL2_MIXER_ROOT`: Path to SDL2_mixer root
- `CMAKE_PREFIX_PATH`: Path to Qt5 toolset (e.g. ` <qt_root>\Qt5.10.1\5.10.1\msvc2017_64`)

Maybe you must define your compiler prefix: e.g. `-DBoost_COMPILER="-vc141"`

Run CMake to generate Visual Studio project files:

```
cmake -G"Visual Studio 15 2017 Win64" -DCMAKE_PREFIX_PATH="<qt_path>" ..
```

If you don't want to install Qt5, add `-DBUILD_EDITOR=OFF` and remove `-DCMAKE_PREFIX_PATH="..."`.

## CMake arguments

| Option                     | Description                              | Default |
| -------------------------- | ---------------------------------------- | ------- |
| BUILD_EDITOR               | Build with editor                        | ON      |
| ENABLE_SOUND               | Build with SDL2 mixer/sound              | ON      |
| BOX2D_USE_SHARED           | Use the shared Box2D library on Linux; otherwise build static Box2D | ON      |
| BUILD_TESTS                | Build with tests                         | ON      |
| ENABLE_WARNINGS_AS_ERRORS  | Treat warnings as errors                 | OFF     |
| ENABLE_ASAN                | Enable address sanitizer                 | OFF     |
| ENABLE_UBSAN               | Enable undefined sanitizer               | OFF     |
| ENABLE_SANITIZER_BLACKLIST | Enable sanitizer blacklist - must be created manually | OFF     |

## Command line arguments

You can specify `-d` or `--debug` for disable checking if one team is empty. Usefull for debugging and testing, if you just have one input device registered. With `-s` or `--no-sound` the sound will be deactivated. Also you can give the path to the game file. Default search paths are `../res/game.xml`, `../../res/game.xml` and `~/.CaptureTheBanana/res/game.xml`. The order is not important.
