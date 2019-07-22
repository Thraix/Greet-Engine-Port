#include "GUIUtils.h"

#include <utils/xml/XMLException.h>

namespace Greet
{
  bool GUIUtils::GetBooleanFromXML(const XMLObject& object, const std::string& key, bool defaultValue)
  {
    if(object.HasAttribute(key))
      return GetBoolean(object.GetAttribute(key));
    return defaultValue;
  }

  Vec4 GUIUtils::GetColorFromXML(const XMLObject& object, const std::string& key, const Vec4& defaultValue)
  {
    if(object.HasAttribute(key))
      return GetColor(object.GetAttribute(key));
    return defaultValue;
  }

  float GUIUtils::GetSizeFromXML(const XMLObject& object, const std::string& key, float defaultValue, float parentSize)
  {
    if(object.HasAttribute(key))
      return GetSize(object.GetAttribute(key), parentSize);
    return defaultValue;
  }
  std::string GUIUtils::GetStringFromXML(const XMLObject& object, const std::string& key, const std::string& defaultValue)
  {
    if(object.HasAttribute(key))
      return object.GetAttribute(key);
    return defaultValue;
  }
  int GUIUtils::GetIntFromXML(const XMLObject& object, const std::string& key, int defaultValue)
  {
    if(object.HasAttribute(key))
      return atoi(object.GetAttribute(key).c_str());
    return defaultValue;
  }

  float GUIUtils::GetFloatFromXML(const XMLObject& object, const std::string& key, float defaultValue)
  {
    if(object.HasAttribute(key))
      return atof(object.GetAttribute(key).c_str());
    return defaultValue;
  }


  bool GUIUtils::GetBoolean(const std::string& str)
  {
    if (str == "true")
      return true;
    return false;
  }


  Vec4 GUIUtils::GetColor(const std::string& str)
  {
    if (str[0] == '#')
    {
      std::string color = str.substr(1);
      if (color.length() != 6 && color.length() != 8)
      {
        Log::Error("Invalid length for color: ", str);
        return Vec4(1, 0, 1, 1); // Invalid color pink since its very visible
      }
      if (color.length() == 6)
        color = "FF" + color; // Add alpha to color
      uint colori = LogUtils::HexToDec(color);
      return ColorUtils::ColorHexToVec4(colori);
    }
    Log::Error("Invalid starting character for  color: ", str);
    return Vec4(1, 0, 1, 1); // Invalid color pink since its very visible
  }

  bool GUIUtils::IsPercentageSize(const std::string& size)
  {
    return StringUtils::ends_with(size, "%");
  }

  bool GUIUtils::IsStaticSize(const std::string& size)
  {
    return !StringUtils::ends_with(size, "%") || StringUtils::ends_with(size, "s%");
  }

  float GUIUtils::GetSize(const std::string& size, float parentSize)
  {
    float sizeF = atof(size.c_str());
    if (StringUtils::ends_with(size, "%"))
    {
      return sizeF * parentSize * 0.01f; // Convert 100 -> 1
    }
    else // if (StringUtils::ends_with(size, "px")) // if there is no ending it counts as pixels.
    {
      return sizeF;
    }
  }
}
