#pragma once

#include <FreeImage.h>
#include <internal/GreetTypes.h>

namespace Greet { namespace ImageFactory {
  BYTE* GetBadFormatImage(uint* width, uint* height);
  BYTE* GetBadBPPImage(uint* width, uint* height);
  BYTE* GetCantReadImage(uint* width, uint* height);
  BYTE* GetCropErrorImage(uint* width, uint* height);
  BYTE* GetErrorImage(uint* width, uint* height, uint lightColor, uint darkColor);
  BYTE* GetFrameImage(uint* width, uint* height);
}}
