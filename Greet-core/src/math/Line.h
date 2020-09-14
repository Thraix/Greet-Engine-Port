#pragma once

#include <math/Vec3.h>

namespace Greet
{
  struct Line
  {
    Vec3<float> pos;
    Vec3<float> dir;

    Line()
      : pos{}, dir{0, 1, 0}
    {}

    Line(const Vec3<float>& pos, const Vec3<float>& dir)
      : pos{pos}, dir{dir}
    {}

    Line Points(const Vec3<float>& p1, const Vec3<float>& p2)
    {
      return Line{p1, p2 - p1};
    }
  };
}

