# Greet Engine Cross-Platform Port 
This is a cross-platform port of the [Greet-Engine](https://github.com/Thraix/Greet-Engine). At some point (when everything is ported) this will be the official version of the engine.

## Installing dependencies 
Below is the commands (on linux) to download and install the different dependencies of this project. These are directly available on the Windows platform.

### Install FreeType2
This library allowes the engine to load fonts to easier be able to render them.
```
  sudo add-apt-repository ppa:glasen/freetype2
  sudo apt update
  sudo apt install freetype2-demos
```

### Install FreeImage
This library allows the engine to load images to easier be able to render them.
```
  sudo apt-get install libfreeimage3 libfreeimage-dev
```

### Install GLFW 
This library offers a cross-platform way to handle windows and input.

To get version 3.2.1 of glfw run the following:

**NOTE: This will replace any newer or older version of the glfw library.**
```
  sudo sh glfwInstall.sh
```

Alternative if you do not trust the script run the following (which is a copy of the script):
```
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
```

### Install GLEW
This library allows the engine to call OpenGL functions.
```
  sudo apt-get install libglew-dev
```


### Install PortAudio
The Greet-Engine currently doesn't use this audio library but is going to be used in the future (probably).
```
  sudo apt-get install libasound-dev
```
