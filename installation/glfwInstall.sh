#!/bin/bash
set -e
git clone git@github.com:glfw/glfw.git
cd glfw
git checkout 0be4f3f75aebd9d24583ee86590a38e741db0904
mkdir build
cd build
cmake ../ -DBUILD_SHARED_LIBS=ON
sudo make install
cd ../../
rm glfw -rf

