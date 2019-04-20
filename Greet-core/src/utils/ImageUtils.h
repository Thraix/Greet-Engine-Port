#pragma once

#include <internal/GreetTypes.h>
#include <internal/GreetGL.h>

namespace Greet {
  struct ImageUtils
  {
    static void PrintImage(BYTE* bits, uint width, uint height);
    static BYTE* FlipImage(BYTE* bits, uint width, uint height);
    static bool LoadImage(const char* filepath, BYTE*& bits, uint* width, uint* height);
    static BYTE* CropImage(const BYTE* bits, uint width,  uint height,  uint cx,  uint cy,  uint cwidth,  uint cheight);
    static void SaveImageBytes(const char* filepath, const char* output);
    static BYTE* CreateHeightmapImage(float* heightMap, uint width, uint height);
  };
}
