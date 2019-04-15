#include "ImageFactory.h"

#include <logging/Log.h>
#include <utils/Utils.h>


namespace Greet { namespace ImageFactory 
  {
    BYTE* GetBadFormatImage(uint* width, uint* height)
    {
      return GetErrorImage(width,height,0xff00ff,0xaa00aa);
    }

    BYTE* GetBadBPPImage(uint* width, uint* height)
    {
      return GetErrorImage(width,height,0xffff00,0xaaaa00);
    }

    BYTE* GetCantReadImage(uint* width, uint* height)
    {
      return GetErrorImage(width,height,0x00ffff,0x00aaaa);
    }

    BYTE* GetCropErrorImage(uint* width, uint* height)
    {
      return GetErrorImage(width,height,0xff0000,0xaa0000);
    }

    BYTE* GetErrorImage(uint* width, uint* height, uint lightColor, uint darkColor)
    {
      *width = 16;
      *height = 16;
      BYTE* bits = new BYTE[*width * *height * 4];
      BYTE* ptr = bits;
      for(uint y = 0;y<*height;y++)
      {
        for(uint x = 0;x<*width;x++)
        {
          uint color = lightColor;
          if(x == 0 || y == *height-1)
            color = darkColor;
          ptr[FI_RGBA_RED] = (color & 0xff0000) >> 16;
          ptr[FI_RGBA_GREEN] = (color & 0xff00) >> 8;
          ptr[FI_RGBA_BLUE] = (color & 0xff) >> 0;
          ptr[FI_RGBA_ALPHA] = 0xff;
          ptr += 4;
        }
      }
      return bits;
    }
  }}
