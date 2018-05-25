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
```
  sudo apt-get install libglfw3-dev
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
