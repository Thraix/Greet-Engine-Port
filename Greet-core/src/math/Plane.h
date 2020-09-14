#pragma once

#include <math/Vec3.h>
#include <math/Line.h>

namespace Greet
{
  class Plane
  {
  public:
    Vec3<float> normal;
    float d;

    Plane()
      : normal{0, 1, 0}, d{0}
    {}

    Plane(const Vec3<float>& normal, float d)
      : normal{Vec::Normalize(normal)}, d{d}
    {}

    Plane(const Vec3<float>& normal, const Vec3<float>& p)
      : normal{Vec::Normalize(normal)}, d{Vec::Dot(Vec::Normalize(normal), p)}
    {}

    static Plane Vertices(const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3)
    {
      Vec3<float> normal = Vec::Cross(p2 - p1, p3 - p1).Normalize();
      float d = Vec::Dot(normal, p1);
      return Plane(normal, d);
    }

    static Plane Span(const Vec3<float>& p1, const Vec3<float>& v1, const Vec3<float>& v2)
    {
      Vec3<float> normal = Vec::Cross(v1, v2).Normalize();
      float d = Vec::Dot(normal, p1);
      return Plane(normal, d);
    }

    Vec3<float> LineIntersection(const Line& line)
    {
      float t = (d - Vec::Dot(normal, line.pos)) / Vec::Dot(normal, line.dir);
      return line.pos + line.dir * t;
    }

    Vec3<float> ProjectPoint(const Vec3<float>& p)
    {
      return p + normal * (d - Vec::Dot(normal, p));
    }

    friend std::ostream& operator<<(std::ostream& stream, const Plane& plane)
    {
      return stream << plane.normal.x << "x + " << plane.normal.y << "y + " << plane.normal.z << "z = " << plane.d;
    }
  };
}

