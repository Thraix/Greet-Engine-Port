#include "ImageUtils.h"

#include <logging/Log.h>
#include <iostream>
#include <graphics/textures/ImageFactory.h>
#include <fstream>
#include <utils/ErrorHandle.h>
#include <utils/StringUtils.h>

namespace Greet {

  void ImageUtils::printImage(BYTE* bits, uint width, uint height)
  {
    for (uint y = 0;y<height;y++)
    {
      BYTE* pixel = (BYTE*)bits;
      for (uint x = 0;x<width;x++)
      {
        std::string s = "pixel(" + StringUtils::to_string(x) + "," + StringUtils::to_string(y) + ")";
        s += "(" + StringUtils::to_string((uint)pixel[FI_RGBA_RED]) + ",";
        s += StringUtils::to_string((uint)pixel[FI_RGBA_GREEN]) + ",";
        s += StringUtils::to_string((uint)pixel[FI_RGBA_BLUE]) + ")";
        pixel += 4;
        Log::Info(s);
      }
      bits += 4*width;
    }
  }

  BYTE* ImageUtils::flipImage(BYTE*& bits, uint width, uint height)
  {
    BYTE* result = new BYTE[width*height*4];
    result += 4*width*height;
    BYTE* row = (BYTE*)bits;
    for (uint y = 0;y < height;y++)
    {
      for (uint x = 0;x < width;x++)
      {
        result -= 4;
        memcpy(result,row,4);
        row += 4;
      }
    }

    delete[] bits;
    bits = result;
    return bits;
  }

  BYTE* ImageUtils::loadImage(const char* filepath, uint* width, uint* height)
  {
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

    FIBITMAP *dib = nullptr;

    fif = FreeImage_GetFileType(filepath, 0);

    if (fif == FIF_UNKNOWN)
      fif = FreeImage_GetFIFFromFilename(filepath);

    if (fif == FIF_UNKNOWN)
    {
      Log::Error("FreeImage file format is not supported or file not exist: ", filepath);
      ErrorHandle::SetErrorCode(GREET_ERROR_IMAGE_FORMAT);
      return ImageFactory::GetBadFormatImage(width,height);
    }

    if (FreeImage_FIFSupportsReading(fif))
      dib = FreeImage_Load(fif, filepath);
    if (!dib)
    {
      Log::Error("FreeImage file Cannot be read: ", filepath);
      ErrorHandle::SetErrorCode(GREET_ERROR_IMAGE_READ);
      return ImageFactory::GetCantReadImage(width,height);
    }


    BYTE* bits = FreeImage_GetBits(dib);

    *width = FreeImage_GetWidth(dib);
    *height = FreeImage_GetHeight(dib);
    uint bpp = FreeImage_GetBPP(dib);
    bpp >>= 3;
    if (bpp != 3 && bpp != 4)
    {
      Log::Error("Bits per pixel is not valid (24 or 32): ", filepath);
      ErrorHandle::SetErrorCode(GREET_ERROR_IMAGE_BPP);
      FreeImage_Unload(dib);
      return ImageFactory::GetBadBPPImage(width,height);
    }

    BYTE* result = new BYTE[(*width) * (*height) * 4];
    uint resultI = 0;
    uint bitsI = 0;
    for(uint y = 0; y < *height; y++)
    {
      for(uint x = 0; x < *width; x++)
      {
        // Incase the order of FreeImage is not RGB (its probably BGRA)
        result[resultI++] = bits[bitsI + FI_RGBA_RED];
        result[resultI++] = bits[bitsI + FI_RGBA_GREEN];
        result[resultI++] = bits[bitsI + FI_RGBA_BLUE];
        if(bpp == 4)
          result[resultI++] = bits[bitsI + FI_RGBA_ALPHA];
        else
          result[resultI++] = 0xff; 
        bitsI += bpp;
      }
    }
    FreeImage_Unload(dib);
    return result;
  }

  BYTE* ImageUtils::cropImage(const BYTE* bits, uint width,  uint height,  uint cx,  uint cy,  uint cwidth,  uint cheight)
  {
    if (cx >= width || cy >= height || cx + cwidth > width || cy + cheight > height)
    {
      Log::Error("Invalid bounds when cropping image. ", cx, ", ", cy, ", ", cwidth, ", ", cheight);
      ErrorHandle::SetErrorCode(GREET_ERROR_IMAGE_CROP);
      return ImageFactory::GetCropErrorImage(&width,&height);
    }
    cy = height - cheight - cy;
    BYTE* result = new BYTE[cwidth * cheight * 4];
    bits += (cx + cy * width) * 4;
    for (uint y = 0;y < cheight;y++)
    {
      memcpy(result, bits, cwidth * 4);
      bits += width * 4;
      result += cwidth * 4;
    }
    result -= cwidth*4*cheight;
    bits -= width*4*cheight;
    return result;
  }

  void ImageUtils::saveImageBytes(const char* filepath, const char* output)
  {
    uint width,height,bpp;
    BYTE* bits = loadImage(filepath,&width,&height);

    std::ofstream fout;
    fout.open(output, std::ios_base::binary | std::ios_base::out);
    fout.write((char*) &bits, width*height*bpp/8);

    fout.close();
  }
}
