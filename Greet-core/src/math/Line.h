#pragma once

#include <math/Vec3.h>

namespace Greet
{
  struct Line
  {
    Vec3f pos;
    Vec3f dir;

    Line()
      : pos{}, dir{0, 1, 0}
    {}

    Line(const Vec3f& pos, const Vec3f& dir)
      : pos{pos}, dir{dir}
    {}

    Line Points(const Vec3f& p1, const Vec3f& p2)
    {
      return Line{p1, p2 - p1};
    }
  };
}

