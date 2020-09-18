#pragma once

#include <math/Vec2.h>
#include <math/Vec3.h>

namespace Greet{
  struct Mat3
  {
    union
    {
      float elements[3 * 3];
      struct
      {
        Vec3f columns[3];
      };
    };

    Mat3();
    Mat3(float diagonal);
    Mat3(float* elem);

    const Vec3f& getColumn(int index)
    {
      return columns[index];
    }

    static Mat3 Identity();
    static Mat3 Orthographic(float left, float right, float top, float bottom);
    static Mat3 OrthographicViewport();
    static Mat3 Quad(float x, float y, float width, float height);
    static Mat3 Quad(const Vec2f& pos, const Vec2f& size);

    static Mat3 Translate(const Vec2f& translation);
    static Mat3 Scale(const Vec2f& scaling);
    static Mat3 Shear(const Vec2f& shearing);

    static Mat3 Translate(const float& x, const float& y);
    static Mat3 Scale(const float& x, const float& y);
    static Mat3 Rotate(float rad);
    static Mat3 Shear(const float& x, const float& y);

    static Mat3 Inverse(const Mat3& mat);
    Mat3 Cpy();

    Mat3& Multiply(const Mat3& other);
    Vec2f Multiply(const Vec2f& other) const;
    Vec3f Multiply(const Vec3f& other) const;

    friend Mat3 operator*(Mat3 first, const Mat3 &second);
    Mat3& operator*=(const Mat3 &other);

    friend Vec2f operator*(const Mat3& first, const Vec2f& second);
    friend Vec3f operator*(const Mat3& first, const Vec3f& second);
    friend Mat3 operator~(const Mat3& first);
  };
}
