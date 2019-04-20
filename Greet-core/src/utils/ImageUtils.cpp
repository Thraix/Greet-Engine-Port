#include "ImageUtils.h"

#include <logging/Log.h>
#include <iostream>
#include <graphics/textures/ImageFactory.h>
#include <fstream>
#include <utils/ErrorHandle.h>
#include <utils/StringUtils.h>

namespace Greet {

  void ImageUtils::PrintImage(BYTE* bits, uint width, uint height)
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

  BYTE* ImageUtils::FlipImage(BYTE* bits, uint width, uint height)
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

    return result;
  }

  bool ImageUtils::LoadImage(const char* filepath, BYTE*& bits, uint* width, uint* height)
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
      bits = ImageFactory::GetBadFormatImage(width,height);
      return false;
    }

    if (FreeImage_FIFSupportsReading(fif))
      dib = FreeImage_Load(fif, filepath);
    if (!dib)
    {
      Log::Error("FreeImage file Cannot be read: ", filepath);
      ErrorHandle::SetErrorCode(GREET_ERROR_IMAGE_READ);
      bits = ImageFactory::GetCantReadImage(width,height);
      return false;
    }


    BYTE* fiBits = FreeImage_GetBits(dib);

    *width = FreeImage_GetWidth(dib);
    *height = FreeImage_GetHeight(dib);
    uint bpp = FreeImage_GetBPP(dib);
    bpp >>= 3;
    if (bpp != 3 && bpp != 4)
    {
      Log::Error("Bits per pixel is not valid (24 or 32): ", filepath);
      ErrorHandle::SetErrorCode(GREET_ERROR_IMAGE_BPP);
      FreeImage_Unload(dib);
      bits = ImageFactory::GetBadBPPImage(width,height);
      return false;
    }

    bits = new BYTE[(*width) * (*height) * 4];
    uint resultI = 0;
    uint bitsI = 0;
    for(uint y = 0; y < *height; y++)
    {
      for(uint x = 0; x < *width; x++)
      {
        // Incase the order of FreeImage is not RGB (its probably BGRA)
        bits[resultI++] = fiBits[bitsI + FI_RGBA_RED];
        bits[resultI++] = fiBits[bitsI + FI_RGBA_GREEN];
        bits[resultI++] = fiBits[bitsI + FI_RGBA_BLUE];
        if(bpp == 4)
          bits[resultI++] = fiBits[bitsI + FI_RGBA_ALPHA];
        else
          bits[resultI++] = 0xff; 
        bitsI += bpp;
      }
    }
    FreeImage_Unload(dib);
    return true;
  }

  BYTE* ImageUtils::CreateHeightmapImage(float* heightMap, uint width, uint height)
  {
    BYTE* data = new BYTE[width * height * 4];
    for(int i = 0;i<width*height;i++)
    {
        data[i * 4 + FI_RGBA_RED]   = heightMap[i] * 255;
        data[i * 4 + FI_RGBA_GREEN] = heightMap[i] * 255;
        data[i * 4 + FI_RGBA_BLUE]  = heightMap[i] * 255;
        data[i * 4 + FI_RGBA_ALPHA] = 0xff;
    }
    return data;
  }

  BYTE* ImageUtils::CropImage(const BYTE* bits, uint width,  uint height,  uint cx,  uint cy,  uint cwidth,  uint cheight)
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

  void ImageUtils::SaveImageBytes(const char* filepath, const char* output)
  {
    uint width,height,bpp;
    BYTE* bits;
    LoadImage(filepath, bits, &width, &height);

    std::ofstream fout;
    fout.open(output, std::ios_base::binary | std::ios_base::out);
    fout.write((char*) &bits, width*height*bpp/8);

    fout.close();
  }
}
