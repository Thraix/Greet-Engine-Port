git clone git@github.com:rougier/freetype-gl.git
cd freetype-gl
mkdir build
cd build
cmake ../ -Dfreetype-gl_BUILD_DEMOS=OFF -Dfreetype-gl_BUILD_TESTS=OFF -Dfreetype-gl_BUILD_APIDOC=OFF
make
cp -f libfreetype-gl.a ../../../deps/libs/unix/
cd ../..
rm -rf freetype-gl

