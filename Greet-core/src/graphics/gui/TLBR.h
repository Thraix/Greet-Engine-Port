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
        if (object.HasProperty(prefix))
        {
          std::vector<std::string> strings = StringUtils::split_string(object.GetProperty(prefix)," ");
          if(strings.size() == 4)
          {
            top = GUIUtils::CalcSize(strings.at(0), baseHeight);
            left = GUIUtils::CalcSize(strings.at(1), baseWidth);
            bottom = GUIUtils::CalcSize(strings.at(2), baseHeight);
            right = GUIUtils::CalcSize(strings.at(3), baseWidth);
          }
          else if(strings.size() == 2)
          {
            top = bottom = GUIUtils::CalcSize(strings.at(0), baseHeight);
            left = right = GUIUtils::CalcSize(strings.at(1), baseWidth);
          }
          else if(strings.size() == 1)
          {
            top = bottom = GUIUtils::CalcSize(strings.at(0), baseHeight);
            left = right = GUIUtils::CalcSize(strings.at(0), baseHeight);
          }
          else
          {
            Log::Error("TLBR:: Invalid parameter for ", prefix);
          }
        }
        else
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

      }

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
  };
}
