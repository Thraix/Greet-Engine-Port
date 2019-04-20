#include "Noise.h"
#include <string>
#include <cmath>
#include <cstring>
#include <logging/Log.h>
#include <math/MathFunc.h>
#include <unordered_map>

namespace Greet {

  float Noise::Eval(int x, int y,uint width, uint height, float stepX, float stepY, uint octaves, float persistance)
  {

    float totalPersistance = 0;
    float p = persistance;
    // Calculate the total persistance to normalize the end value
    for(int i = 0;i<octaves;i++)
    {
      totalPersistance += p;
      p*=p;
    }
    float value = 0.0f;

    for(int i = 0;i<octaves;i++)
    {
      // Convert to 1 step grid system.
      float xx = x / stepX; 
      float yy = y / stepY; 

      // Retrieve the 4 noise points round the "pixel" 
      float tl = Smooth(xx, yy, i);
      float tr = Smooth(xx+1, yy, i);
      float bl = Smooth(xx, yy+1, i);
      float br = Smooth(xx+1, yy+1, i);

      // Interpolate the 4 values
      value += Interpolate(tl, tr, bl, br, xx - (int)xx, yy - (int)yy) * persistance;

      // Decrease the step by half
      // This should maybe be a parameter in the future
      stepX *= 0.5f;
      stepY *= 0.5f;
      persistance *= persistance;
    }
    return value / totalPersistance;
  }

  float Noise::Smooth(int x, int y, uint octave)
  {
    // Smooth values ( center * 0.5 + edges * 0.08333 + corners * 0.0416666)
    return PRNG(x, y, octave) * 0.5 +
      (PRNG(x-1, y, octave) + PRNG(x+1, y, octave) + PRNG(x, y-1, octave) + PRNG(x, y+1, octave)) * 0.08333f +
      (PRNG(x-1, y-1, octave) + PRNG(x+1, y-1, octave) + PRNG(x-1, y+1, octave) + PRNG(x+1, y+1, octave)) * 0.0416666f;
  }

  float* Noise::GenNoise(uint width, uint height, uint octave, uint stepX, uint stepY, float persistance, int offsetX, int offsetY)
  {
    float* result = new float[width * height];

    // Calculate the noise for each "pixel"
    // Could be improved by not computing the same noise values multiple timers
    for(int y = 0;y<height;y++)
    {
      for(int x = 0;x<width;x++)
      {
        result[x + y * width] = Eval(x + offsetX, y + offsetY, width, height, stepX, stepY, octave, persistance);
      }
    }
    return result;
  }

  float Noise::PRNG(int x, int y, int octave)
  {
    // hash the values to get a better "random" n value
    size_t n = std::hash<int>()(x * 34184077) ^ std::hash<int>()(y * 15487067) ^ std::hash<int>()(octave * 87855217);
    return (double)( ( n * (n * n * 20947 + 794327) + 1376312589) & 0x7fffffff) / 1073741823.0*0.5;
  }

  // 2d interpolation
  float Noise::Interpolate(float v1, float v2, float v3, float v4, float d1, float d2)
  {
    return Interpolate(Interpolate(v1, v2, d1), Interpolate(v3, v4, d1), d2);
  }

  // 1d interpolation
  float Noise::Interpolate(float v1, float v2, float d)
  {
    float c = 0.5f+cos(d * M_PI) * 0.5f;
    float v = (v1 - v2)*c + v2;

    return v;
  }
}
