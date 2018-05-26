wget https://github.com/glfw/glfw/archive/3.2.1.zip
unzip 3.2.1.zip
cd glfw-3.2.1
mkdir build
cd build
cmake ../ -DBUILD_SHARED_LIBS=ON
make
sudo cp src/libglfw.so* /usr/lib/x86_64-linux-gnu/
cd ../../
rm glfw-3.2.1 -rf
rm 3.2.1.zip
