#pragma once

#include <internal/GreetTypes.h>

namespace Greet {
  class Noise
  {

    public:

    private:
      Noise() = delete;
      Noise(const Noise& noise) = delete;

    public:
      /*
       * width = width of the noise
       * height = height of the noise
       * octave = number of iterations the noise should go through
       * stepX = number of pixels in the first iteration in the x axis
       * stepY = number of pixels in the first iteration in the y axis
       */
      static float* GenNoise(uint width, uint height, uint octave, uint stepX, uint stepY, float persistance, int offsetX=0, int offsetY=0);

      // 3D
      static float* GenNoise(uint width, uint height, uint length, uint octave, uint stepX, uint stepY, uint stepZ, float persistance, int offsetX, int offsetY, int offsetZ);
    private:
      static float Smooth(int x, int y, uint octave);
      static float Eval(int x, int y, uint width, uint height, float stepX, float stepY, uint octaves, float persistance);

      // 3D
      static float Smooth(int x, int y, int z, uint octave);
      static float Eval(int x, int y, int z, uint width, uint height, uint length, float stepX, float stepY, float stepZ, uint octaves, float persistance);
    public:
      static float PRNG(int x, int y, int z, int octave);
      static float PRNG(int x, int y, int octave);

      static float Interpolate(float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float d1, float d2, float d3);
      static float Interpolate(float v1, float v2, float v3, float v4, float d1, float d2);
      static float Interpolate(float v1, float v2, float d);
  };
}
