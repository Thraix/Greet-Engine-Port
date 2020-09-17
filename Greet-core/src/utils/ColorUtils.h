#pragma once

#include <common/Types.h>
#include <math/Maths.h>
#include <graphics/Color.h>
#include <logging/Logger.h>

#define COLOR_UTILS_MODIFIER 0.1f

namespace Greet {

  struct ColorUtils
  {
    static Color GetMaterialColorAsHSV(float hue, int level)
    {
      float p = powf(1.0f - COLOR_UTILS_MODIFIER, level);
      return Color(hue,p,p,1);
    }

    static Color GetMaterialColorAsRGB(float hue, int level)
    {
      float p = powf(1.0f-COLOR_UTILS_MODIFIER, level);
      return Color(hue,p,p,1).ToRGB();
    }
  };
}
