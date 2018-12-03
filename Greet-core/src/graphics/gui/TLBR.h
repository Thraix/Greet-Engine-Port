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
            top = GUIUtils::GetSize(strings.at(0), baseHeight);
            left = GUIUtils::GetSize(strings.at(1), baseWidth);
            bottom = GUIUtils::GetSize(strings.at(2), baseHeight);
            right = GUIUtils::GetSize(strings.at(3), baseWidth);
          }
          else if(strings.size() == 2)
          {
            top = bottom = GUIUtils::GetSize(strings.at(0), baseHeight);
            left = right = GUIUtils::GetSize(strings.at(1), baseWidth);
          }
          else if(strings.size() == 1)
          {
            top = bottom = GUIUtils::GetSize(strings.at(0), baseHeight);
            left = right = GUIUtils::GetSize(strings.at(0), baseHeight);
          }
          else
          {
            Log::Error("TLBR:: Invalid parameter for ", prefix);
          }
        }
        else
        {
          top = GUIUtils::GetSizeFromXML(object, prefix+"Top", top, baseHeight);
          left = GUIUtils::GetSizeFromXML(object, prefix+"Left", top, baseWidth);
          bottom = GUIUtils::GetSizeFromXML(object, prefix+"Bottom", top, baseHeight);
          right = GUIUtils::GetSizeFromXML(object, prefix+"Right", top, baseWidth);
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

      Vec2 GetSize() const
      {
        return Vec2(GetWidth(), GetHeight());
      }
  };
}
