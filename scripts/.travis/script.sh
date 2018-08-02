#!/bin/bash
set -ex

mkdir -p build
pushd build

# Treat warnings as errors
#export CXXFLAGS="-DENABLE_WARNINGS_AS_ERRORS=ON:$CXXFLAGS"

# Build flags
if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
    CTB_FLAGS="-DCMAKE_BUILD_TYPE=Debug -DBUILD_GAME_EDITOR=ON -DBUILD_TESTS=ON"
else
    CTB_FLAGS="-DCMAKE_BUILD_TYPE=Debug -DBUILD_GAME_EDITOR=ON -DBUILD_TESTS=ON"

    # Qt environment paths
    export PATH="/usr/local/opt/qt/bin:$PATH"
    export CMAKE_PREFIX_PATH="/usr/local/opt/qt:$CMAKE_PREFIX_PATH"
fi

# Build
cmake $CTB_FLAGS ..
make -j2

# Print version
ctb/CaptureTheBanana -v

# Run unit tests
tests/unit-tests

popd # build

exit 0
