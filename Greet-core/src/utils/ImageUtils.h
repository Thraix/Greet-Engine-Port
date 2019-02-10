#pragma once

#include <internal/GreetTypes.h>
#include <internal/GreetGL.h>

namespace Greet {
  struct ImageUtils
  {
    static void printImage(BYTE* bits, uint width, uint height);
    static BYTE* flipImage(BYTE*& bits, uint width, uint height);
    static BYTE* loadImage(const char* filepath, uint* width, uint* height);
    static BYTE* cropImage(const BYTE* bits, uint width,  uint height,  uint cx,  uint cy,  uint cwidth,  uint cheight);
    static void saveImageBytes(const char* filepath, const char* output);
  };
}
