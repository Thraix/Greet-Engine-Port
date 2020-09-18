#pragma once

#include <ostream>

namespace Greet {

  template <typename T>
  struct Vec3;
  typedef Vec3<float> Vec3f;

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
      Vec3f Multiply(const Vec3f& other);

      static Quaternion AxisAngle(const Vec3f& axis, float angle);

      friend Quaternion operator*(const Quaternion& first, const Quaternion& second);
      friend Vec3f operator*(const Quaternion& first, const Vec3f& second);

      Quaternion& operator*=(const Quaternion& other);
      Quaternion& operator*=(const Vec3f & other);

      friend std::ostream& operator<<(std::ostream& stream, const Quaternion& q)
      {
        return stream << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
      }
  };

}
