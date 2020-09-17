#pragma once

#include <cmath>
#include <iostream>
#include <math/Vec3.h>
#include <math/Vec4.h>

namespace Greet{

  template <typename T>
  struct Vec2
  {
    public:
      union
      {
        struct
        {
          float x, y;
        };
        struct
        {
          float w, h;
        };
        struct
        {
          float s, t;
        };
      };
      Vec2()
        : x{0}, y{0}
      {}

      Vec2(float f)
        : x{f}, y{f}
      {}

      Vec2(float x, float y)
        : x{x}, y{y}
      {}

      Vec2(const Vec3<float>& vec3)
        : x(vec3.x), y(vec3.y)
      {}

      Vec2(const Vec4& vec4)
        : x(vec4.x), y(vec4.y)
      {

      }

      float Length() const
      {
        return sqrt(x*x + y*y);
      }

      float LengthSQ() const
      {
        return x*x + y*y;
      }

      float Dot(const Vec2& vec) const
      {
        return x*vec.x + y*vec.y;
      }

      float Cross(const Vec2& vec) const
      {
        return x * vec.y - y * vec.x;
      }

      Vec2& Abs()
      {
        x = x < 0 ? -x : x;
        y = y < 0 ? -y : y;
        return *this;
      }

      Vec2& Projected(const Vec2& vec)
      {
        return Multiply(Dot(vec) / LengthSQ());
      }

      Vec2& Normalize()
      {
        float len = Length();
        if(len == 0)
          return *this;
        x /= len;
        y /= len;
        return *this;
      }

      Vec2& Rotate(float rad)
      {
        float c = cos(rad);
        float s = sin(rad);

        float tempX = x*c - y*s;
        float tempY = x*s + y*c;
        x = tempX;
        y = tempY;

        return *this;
      }

      Vec2& Add(const Vec2& other)
      {
        x += other.x;
        y += other.y;
        return *this;
      }

      Vec2& Subtract(const Vec2& other)
      {
        x -= other.x;
        y -= other.y;
        return *this;
      }

      Vec2& Multiply(const Vec2& other)
      {
        x *= other.x;
        y *= other.y;
        return *this;
      }

      Vec2& Divide(const Vec2& other)
      {
        x /= other.x;
        y /= other.y;
        return *this;
      }

      Vec2& Add(const T& c)
      {
        x += c;
        y += c;
        return *this;
      }

      Vec2& Subtract(const T& c)
      {
        x -= c;
        y -= c;
        return *this;
      }

      Vec2& Multiply(const T& c)
      {
        x *= c;
        y *= c;
        return *this;
      }

      Vec2& Divide(const T& c)
      {
        x /= c;
        y /= c;
        return *this;
      }

      bool Compare(const Vec2& other) const
      {
        return x == other.x && y == other.y;
      }

      friend Vec2 operator+(const Vec2& first, const Vec2& second)
      {
        return Vec2(first.x,first.y).Add(second);
      }

      friend Vec2 operator-(const Vec2& first, const Vec2 &second)
      {
        return Vec2(first.x, first.y).Subtract(second);
      }

      friend Vec2 operator*(const Vec2& first, const Vec2 &second)
      {
        return Vec2(first.x, first.y).Multiply(second);
      }

      friend Vec2 operator/(const Vec2& first, const Vec2 &second)
      {
        return Vec2(first.x, first.y).Divide(second);
      }

      friend Vec2 operator+(const Vec2& first, const T& c)
      {
        return Vec2(first.x, first.y).Add(c);
      }

      friend Vec2 operator-(const Vec2& first, const T& c)
      {
        return Vec2(first.x, first.y).Subtract(c);
      }

      friend Vec2 operator*(const Vec2&  first, const T& c)
      {
        return Vec2(first.x, first.y).Multiply(c);
      }

      friend Vec2 operator/(const Vec2& first, const T& c)
      {
        return Vec2(first.x, first.y).Divide(c);
      }

      friend Vec2 operator-(const Vec2& first)
      {
        return Vec2(-first.x, -first.y);
      }

      T& operator[](uint i)
      {
        ASSERT(i < 2, "Index out of bound");
        return *((&x)+i);
      }

      const T& operator[](uint i) const
      {
        ASSERT(i < 2, "Index out of bound");
        return *((&x)+i);
      }

      Vec2& operator+=(const Vec2 &other)
      {
        return Add(other);
      }

      Vec2& operator-=(const Vec2 &other)
      {
        return Subtract(other);
      }

      Vec2& operator*=(const Vec2 &other)
      {
        return Multiply(other);
      }

      Vec2& operator/=(const Vec2 &other)
      {
        return Divide(other);
      }

      Vec2& operator+=(const T& c)
      {
        x += c;
        y += c;
        return *this;
      }

      Vec2& operator-=(const T& c)
      {
        x -= c;
        y -= c;
        return *this;
      }

      Vec2& operator*=(const T& c)
      {
        x *= c;
        y *= c;
        return *this;
      }

      Vec2& operator/=(const T& c)
      {
        x /= c;
        y /= c;
        return *this;
      }

      bool operator==(const Vec2 &other) const
      {
        return Compare(other);
      }

      bool operator!=(const Vec2 &other) const
      {
        return !Compare(other);
      }

      bool operator<(const Vec2& other) const
      {
        if(x == other.x)
          return y < other.y;
        return x < other.x;
      }

      bool operator>(const Vec2& other) const
      {
        if(x == other.x)
          return y > other.y;
        return x > other.x;
      }

      bool operator<=(const Vec2& other) const
      {
        if(x == other.x)
          return y <= other.y;
        return x <= other.x;
      }

      bool operator>=(const Vec2& other) const
      {
        if(x == other.x)
          return y >= other.y;
        return x >= other.x;
      }

      friend std::ostream& operator<<(std::ostream& stream, const Vec2& vec)
      {
        return stream << "(" << vec.x << ", " << vec.y << ")";
      }
  };

  typedef Vec2<float> Vec2f;
  typedef Vec2<int> Vec2i;
}
