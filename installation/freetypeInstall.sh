#!/bin/bash
wget https://download.savannah.gnu.org/releases/freetype/freetype-2.9.tar.gz
tar -xvzf freetype-2.9.tar.gz
cd freetype-2.9
mkdir build
cd build
sudo cmake ../ -DBUILD_SHARED_LIBS:BOOL=true
sudo make install
cd ../../
sudo m -rf freetype-2.9*
