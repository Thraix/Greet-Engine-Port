#include "CubeMap.h"

#include <internal/GreetGL.h>
#include <utils/ImageUtils.h>
#include <utils/ErrorHandle.h>

namespace Greet {
  CubeMap::CubeMap(const std::string& top, const std::string& bottom, const std::string& left, const std::string& right, const std::string& front, const std::string& back)
    : Texture(GL_TEXTURE_CUBE_MAP)
  {
    LoadCubeMap(top, bottom, left, right, front, back);
  }

  CubeMap::CubeMap(const std::string& map)
    : Texture(GL_TEXTURE_CUBE_MAP)
  {
    LoadCubeMap(map);
  }

  CubeMap::CubeMap(uint texId)
    : Texture(texId, (uint)GL_TEXTURE_CUBE_MAP)
  {

  }

  void CubeMap::LoadCubeMap(const std::string& image)
  {
    GLCall(glActiveTexture(GL_TEXTURE0));
    Enable();

    uint width;
    uint height;
    BYTE* bits;
    bool succeed = ImageUtils::LoadImage(image.c_str(), bits, &width, &height);
    if(width % 4 != 0 || height % 3 != 0)
    {
      // If we didn't succeed there is no reason to show more errors
      if(succeed)
        Log::Error("Invalid cube map texture size: ", width, "x", height, " (need to be divisible 4x3)");
      LoadImage(bits,width,height,GL_TEXTURE_CUBE_MAP_POSITIVE_X,false,false);
      LoadImage(bits,width,height,GL_TEXTURE_CUBE_MAP_NEGATIVE_X,false,false);
      LoadImage(bits,width,height,GL_TEXTURE_CUBE_MAP_POSITIVE_Y,false,false);
      LoadImage(bits,width,height,GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,false,false);
      LoadImage(bits,width,height,GL_TEXTURE_CUBE_MAP_POSITIVE_Z,false,false);
      LoadImage(bits,width,height,GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,false,false);
    }
    else
    {
      uint w = width / 4;
      uint h = height / 3;
      LoadImage(ImageUtils::CropImage(bits,width,height,0,h,w,h),w,h,GL_TEXTURE_CUBE_MAP_POSITIVE_X,true,true);
      LoadImage(ImageUtils::CropImage(bits,width,height,w*2,h,w,h),w,h,GL_TEXTURE_CUBE_MAP_NEGATIVE_X,true,true);

      LoadImage(ImageUtils::CropImage(bits,width,height,w,0,w,h),w,h,GL_TEXTURE_CUBE_MAP_POSITIVE_Y,true,true);
      LoadImage(ImageUtils::CropImage(bits,width,height,w,h*2,w,h),w,h,GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,true,true);

      LoadImage(ImageUtils::CropImage(bits,width,height,w,h,w,h),w,h,GL_TEXTURE_CUBE_MAP_POSITIVE_Z,true,true);
      LoadImage(ImageUtils::CropImage(bits,width,height,w*3,h,w,h),w,h,GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,true,true);
    }
    LoadParameters();
    delete[] bits;

    Disable();
  }

  void CubeMap::LoadCubeMap(const std::string& top, const std::string& bottom, const std::string& left, const std::string& right, const std::string& front, const std::string& back)
  {
    GLCall(glActiveTexture(GL_TEXTURE0));
    Enable();

    LoadImage(right, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    LoadImage(left, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    LoadImage(top, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    LoadImage(bottom, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    LoadImage(back, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    LoadImage(front, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

    LoadParameters();

    Disable();
  }

  void CubeMap::LoadParameters()
  {
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
  }

  void CubeMap::LoadImage(const std::string& image, uint mapLocation)
  {
    uint width;
    uint height;
    BYTE* bits;
    ImageUtils::LoadImage(image.c_str(), bits, &width, &height);
    LoadImage(bits,width,height,mapLocation,true,true);
  }

  void CubeMap::LoadImage(BYTE* bits, uint width, uint height, uint mapLocation, bool freeMem, bool flip)
  {
    BYTE* flippedBits = bits;
    if(flip)
      flippedBits = ImageUtils::FlipImage(bits,width,height);
    GLCall(glTexImage2D(mapLocation, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, flippedBits));
    if(flip)
      delete[] flippedBits;
    if(freeMem)
      delete[] bits;
  }
}
