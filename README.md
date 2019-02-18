# Greet Engine Linux Port 
This is a Linux port of the [Greet-Engine](https://github.com/Thraix/Greet-Engine). At some point (when everything is ported and Windows is working) this will be the official version of the engine.

## Installing dependencies 
Below is the commands (on Ubuntu) to download and install the different dependencies of this project.
```
  sudo add-apt-repository ppa:glasen/freetype2
  sudo apt update
  sudo apt install freetype2-demos libfreeimage3 libfreeimage-dev xorg-dev libglew-dev
```

In case you cannot install the freetype package there is also an installation script to install it.

**NOTE: This will replace any newer or older version of the freetype library.**
```
  cd installation
  sudo sh freetypeInstall.sh
```

### Install GLFW 
This library offers a cross-platform way to handle windows and input.

To get version 3.2.1 of glfw run the following:

**NOTE: This will replace any newer or older version of the glfw library.**
```
  cd installation
  sudo sh glfwInstall.sh
```

### Install PortAudio
The Greet-Engine currently doesn't use this audio library but is going to be used in the future (probably).
```
  sudo apt-get install libasound-dev
```
