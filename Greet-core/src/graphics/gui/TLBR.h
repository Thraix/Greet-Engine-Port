#pragma once

#include <logging/Log.h>
#include <math/Vec4.h>
#include <math/Vec2.h>
#include <utils/xml/XMLObject.h>

#include <cstdlib>

namespace Greet
{
  struct TLBR
  {
    public:
      float top;
      float left;
      float bottom;
      float right;

      TLBR()
        : top{0}, left{0}, bottom{0},right{0}
      {}

      TLBR(float top, float left, float bottom, float right)
        : top{top}, left{left}, bottom{bottom},right{right}
      {}

      friend std::ostream& operator<<(std::ostream& stream, const TLBR& tlbr)
      {
        return stream<< "(" << tlbr.top << ", " << tlbr.left << ", " << tlbr.bottom  << ", "<< tlbr.right << ")";
      }

      Vec2 LeftTop() const
      {
        return Vec2(left, top);
      }

      Vec2 RightBottom() const
      {
        return Vec2(right, bottom);
      }

      float GetWidth() const
      {
        return left + right;
      }

      float GetHeight() const
      {
        return top + bottom;
      }

      Vec2 GetSize() const
      {
        return Vec2(GetWidth(), GetHeight());
      }
  };
}
