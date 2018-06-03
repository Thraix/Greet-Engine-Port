wget https://download.savannah.gnu.org/releases/freetype/freetype-2.9.tar.gz
tar -xvzf freetype-2.9.tar.gz
cd freetype-2.9
mkdir build
cd build
cmake ../ -DBUILD_SHARED_LIBS
make install
cd ../../
rm -rf freetype-2.9*

