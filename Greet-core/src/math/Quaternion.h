#pragma once

#include <ostream>

namespace Greet {

  template <typename Real>
  struct Vec3;

  class Quaternion
  {
    public:
      float x;
      float y;
      float z;
      float w;

    public:
      Quaternion(float x, float y, float z, float w);

      float Length();
      Quaternion& Normalize();
      Quaternion& Conjugate();

      Quaternion& Multiply(const Quaternion& other);
      Vec3<float> Multiply(const Vec3<float>& other);

      static Quaternion AxisAngle(const Vec3<float>& axis, float angle);

      friend Quaternion operator*(const Quaternion& first, const Quaternion& second);
      friend Vec3<float> operator*(const Quaternion& first, const Vec3<float>& second);

      Quaternion& operator*=(const Quaternion &other);
      Quaternion& operator*=(const Vec3<float>  &other);

      friend std::ostream& operator<<(std::ostream& stream, const Quaternion& q)
      {
        return stream << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
      }
  };

}
