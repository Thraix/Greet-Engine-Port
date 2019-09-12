#pragma once

#include <vector>
#include <FreeImage.h>
#include <common/Types.h>

namespace Greet { namespace ImageFactory {
  std::vector<BYTE> GetBadFormatImage(uint* width, uint* height);
  std::vector<BYTE> GetBadBPPImage(uint* width, uint* height);
  std::vector<BYTE> GetCantReadImage(uint* width, uint* height);
  std::vector<BYTE> GetCropErrorImage(uint* width, uint* height);
  std::vector<BYTE> GetErrorImage(uint* width, uint* height, uint lightColor, uint darkColor);
  std::vector<BYTE> GetFrameImage(uint* width, uint* height);
}}
