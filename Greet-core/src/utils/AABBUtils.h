#pragma once

#include <math/Maths.h>

namespace Greet {
  struct AABBUtils
  {
    static bool PointInsideBox(const Vec2f& pointer, const Vec2f& boxPos, const Vec2f& boxSize)
    {
      return pointer.x >= boxPos.x && pointer.x < boxPos.x + boxSize.x && pointer.y >= boxPos.y && pointer.y < boxPos.y + boxSize.y;
    }
  };
}
