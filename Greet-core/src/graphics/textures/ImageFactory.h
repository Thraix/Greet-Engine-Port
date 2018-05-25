#pragma once
#include <logging/Log.h>
#include <FreeImage.h>
#include <internal/GreetTypes.h>
#include <utils/Utils.h>

namespace Greet { namespace ImageFactory {
		BYTE* GetBadFormatImage(uint* width, uint* height, uint* bpp);
		BYTE* GetBadBPPImage(uint* width, uint* height, uint* bpp);
		BYTE* GetCantReadImage(uint* width, uint* height, uint* bpp);
		BYTE* GetCropErrorImage(uint* width, uint* height, uint* bpp);
		BYTE* GetErrorImage(uint* width, uint* height, uint* bpp, uint lightColor, uint darkColor);
		BYTE* GetFrameImage(uint* width, uint* height, uint* bpp);
}}