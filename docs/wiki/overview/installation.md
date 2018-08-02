# Installation

## Supported platforms

- Linux
- macOS
- Windows

## Dependencies

These are the dependencies currently used by CaptureTheBanana++.

- boost 1.57 or above
- Box2D (Linux only)
- SDL2 and SDL2_image
- SDL2 Mixer (optional, for sound)
- Qt v5.9 or above (optional, for editor)

### Debian based distros

```
sudo apt-get install libboost-filesystem libboost-system libbox2d libsdl2 libsdl2-image libsdl2-mixer
```

If you want to use the editor:

```
sudo apt-get install qt5-default
```

If Qt5 version is too old, see [Qt5 ppa](https://launchpad.net/~beineri) and install `qt510-meta-minimal` or use the offical Qt5  installer.

### Fedora

```
sudo dnf install boost Box2D SDL2 SDL2_image SDL2_mixer
```

If you want to use the editor:

```
sudo dnf install qt5
```

### OSX

First of all, you need [Homebrew](https://brew.sh/).

```
brew install sdl2 sdl2_image sdl2_mixer
```

If you want to use the editor:

```
brew install qt
```

### Windows

No dependencies needed.

## Download

You can download CaptureTheBanana++ from [here](https://github.com/CaptureTheBanana/CaptureTheBanana/releases).

## Verify your installation

```
CaptureTheBanana --version
```
