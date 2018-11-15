#pragma once

#include <math/Vec4.h>
#include <utils/xml/XMLObject.h>
#include <graphics/gui/GUIUtils.h>

namespace Greet
{
  struct TLBR
  {
    public:
      float top;
      float left;
      float bottom;
      float right;

      TLBR(float top, float left, float bottom, float right)
        : top(top), left(left), bottom(bottom),right(right)
      {}

      void Load(const std::string& prefix, const XMLObject& object, float baseWidth, float baseHeight)
      {
        if (object.HasProperty(prefix+"Top"))
        {
          top = GUIUtils::CalcSize(object.GetProperty(prefix+"Top"), baseHeight);
        }
        if (object.HasProperty(prefix+"Left"))
        {
          left  = GUIUtils::CalcSize(object.GetProperty(prefix+"Left"), baseWidth);
        }
        if (object.HasProperty(prefix+"Bottom"))
        {
          bottom  = GUIUtils::CalcSize(object.GetProperty(prefix+"Bottom"), baseHeight);
        }
        if (object.HasProperty(prefix+"Right"))
        {
          right = GUIUtils::CalcSize(object.GetProperty(prefix+"Right"), baseWidth);
        }
      
      }

      friend std::ostream& operator<<(std::ostream& stream, const TLBR& tlbr)
      {
        return stream<< "(" << tlbr.top << ", " << tlbr.left << ", " << tlbr.bottom  << ", "<< tlbr.right << ")";
      }

      Vec2 LeftTop()
      {
        return Vec2(left, top);
      }

      Vec2 RightBottom()
      {
        return Vec2(right, bottom);
      }
  };
}
