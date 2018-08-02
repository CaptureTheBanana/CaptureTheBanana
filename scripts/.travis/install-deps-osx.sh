#!/bin/bash
set -ex

# cmake and boost 1.65.1 are already installed
brew update
brew install \
 clang-format \
 sdl2 \
 sdl2_image \
 sdl2_mixer \
 qt

exit 0
