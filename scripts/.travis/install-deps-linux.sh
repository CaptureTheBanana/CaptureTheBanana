#!/bin/bash
set -ex

sudo apt-get -qq update
sudo apt-get -qq -y install \
 cmake \
 clang-format-5.0 \
 libboost1.60-all-dev \
 gcc-7 \
 g++-7 \
 qt510-meta-minimal
 
# sdl2 packages are moved to .travis.yml, due to "unmet dependencie"

sudo ln -s /usr/bin/clang-format-5.0 /usr/bin/clang-format

mkdir -p $HOME/deps

# Install Box2D v2.3.1 (xenial packages)
pushd $HOME/deps
wget -q http://mirrors.kernel.org/ubuntu/pool/universe/b/box2d/libbox2d2.3.0_2.3.1+ds-3_amd64.deb
sha256sum libbox2d2.3.0_2.3.1+ds-3_amd64.deb | awk '$1!="902fe603a1b1c4306de43759322d5d5721229c625eb6693531b13369adaaa672"{ exit 1 }'
sudo dpkg -i libbox2d2.3.0_2.3.1+ds-3_amd64.deb

wget -q http://mirrors.kernel.org/ubuntu/pool/universe/b/box2d/libbox2d-dev_2.3.1+ds-3_amd64.deb
sha256sum libbox2d-dev_2.3.1+ds-3_amd64.deb | awk '$1!="c73079196d89385348673dd0c5b5acebe5b721db0b5f3697ef52fc877622589c"{ exit 1 }'
sudo dpkg -i libbox2d-dev_2.3.1+ds-3_amd64.deb
popd # $HOME/deps

exit 0
