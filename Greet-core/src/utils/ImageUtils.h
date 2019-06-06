#pragma once

#include <internal/GreetTypes.h>
#include <internal/GreetGL.h>

namespace Greet {
  struct ImageUtils
  {
    static void PrintImage(BYTE* bits, uint width, uint height);
    static std::vector<BYTE> FlipImage(const std::vector<BYTE>& bits, uint width, uint height);
    static std::pair<bool, std::vector<BYTE>> LoadImage(const std::string& filepath, uint* width, uint* height);
    static std::vector<BYTE> CropImage(const std::vector<BYTE>& bits, uint width,  uint height,  uint cx,  uint cy,  uint cwidth,  uint cheight);
    static void SaveImageBytes(const std::string& filepath, const std::string& output);
    static std::vector<BYTE> CreateHeightmapImage(const std::vector<float>& heightMap, uint width, uint height);
  };
}
