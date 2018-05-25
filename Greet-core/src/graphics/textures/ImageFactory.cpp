#include "ImageFactory.h"

namespace Greet { namespace ImageFactory 
{
	BYTE* GetBadFormatImage(uint* width, uint* height, uint* bpp)
	{
		return GetErrorImage(width,height,bpp,0xff00ff,0xaa00aa);
	}
	
	BYTE* GetBadBPPImage(uint* width, uint* height, uint* bpp)
	{
		return GetErrorImage(width,height,bpp,0xffff00,0xaaaa00);
	}
	
	BYTE* GetCantReadImage(uint* width, uint* height, uint* bpp)
	{
		return GetErrorImage(width,height,bpp,0x00ffff,0x00aaaa);
	}

	BYTE* GetCropErrorImage(uint* width, uint* height, uint* bpp)
	{
		return GetErrorImage(width,height,bpp,0xff0000,0xaa0000);
	}
	
	BYTE* GetErrorImage(uint* width, uint* height, uint* bpp, uint lightColor, uint darkColor)
	{
		*width = 16;
		*height = 16;
		*bpp = 32;
		uint pitch = 4 * *width;
		BYTE* bits = new BYTE[*width * *height * 4];
		uint add = 0;
		for(uint y = 0;y<*height;y++)
		{
			BYTE* pixel = (BYTE*)bits;
			for(uint x = 0;x<*width;x++)
			{
				uint color = lightColor;
				if(x == 0 || y == *height-1)
					color = darkColor;
				pixel[FI_RGBA_RED] = (color & 0xff0000) >> 16;
				pixel[FI_RGBA_GREEN] = (color & 0xff00) >> 8;
				pixel[FI_RGBA_BLUE] = (color & 0xff) >> 0;
				pixel[FI_RGBA_ALPHA] = 0xff;
				pixel+=4;
			}
			bits += pitch ;
			add += pitch;
		}
		bits -= add;
		return bits;
	}
#if 0
	BYTE* getFrameImage(uint* width, uint* height, uint* bpp)
	{
		const char* c = {
			#include "frame.bin"
		};
		*width = 16;
		*height = 16;
		*bpp = 32;
		BYTE b[1024];
		for(uint i = 0;i<1024;i++)
		{
			b[i] = 0xff;
		}
		delete[] c;
		return b;

	}
#endif
}}
