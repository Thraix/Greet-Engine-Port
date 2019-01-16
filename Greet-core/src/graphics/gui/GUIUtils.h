#pragma once

#include <graphics/Window.h>
#include <utils/StringUtils.h>
#include <utils/ColorUtils.h>
#include <utils/xml/XML.h>

namespace Greet {

  class Frame;
  class Component;

  class GUIUtils
  {
    public:

      static bool GetBooleanFromXML(const XMLObject& object, const std::string& key, bool defaultValue);
      static Vec4 GetColorFromXML(const XMLObject& object, const std::string& key, const Vec4& defaultValue);
      static float GetSizeFromXML(const XMLObject& object, const std::string& key, float defaultValue, float parentSize);
      static std::string GetStringFromXML(const XMLObject& object, const std::string& key, const std::string& defaultValue);
      static int GetIntFromXML(const XMLObject& object, const std::string& key, int defaultValue);
      static float GetFloatFromXML(const XMLObject& object, const std::string& key, float defaultValue);

      static bool GetBoolean(const std::string& str);
      static Vec4 GetColor(const std::string& str);
      static float GetSize(const std::string& size, float parentSize);
      static bool IsPercentageSize(const std::string& size);
      static bool IsStaticSize(const std::string& size);
  };
}
