#include "GUIUtils.h"

#include <graphics/gui/Component.h>
#include <graphics/gui/Frame.h>
#include <graphics/gui/Button.h>
#include <graphics/gui/Label.h>
#include <graphics/gui/ProgressBar.h>
#include <graphics/gui/Container.h>
#include <graphics/gui/Divider.h>
#include <graphics/gui/TextBox.h>

namespace Greet
{
  Frame* GUIUtils::GetFrame(const XMLObject& object)
  {
    if (object.GetName() == "Frame")
    {
      return new Frame(object);
    }
    else if (object.GetName() == "FrameHeader")
    {

    }
    Log::Warning("Could not read XML object ", object.GetName(), ".");
    return new Frame();
  }

  Component* GUIUtils::GetComponent(const XMLObject& object, Component* parent)
  {
    if (object.GetName() == "Button")
    {
      return new Button(object, parent);
    }
    else if(object.GetName() == "Label")
    {
      return new Label(object, parent);
    }
    else if(object.GetName() == "ProgressBar")
    {
      return new ProgressBar(object,parent);
    }
    else if (object.GetName() == "Component")
    {
      return new Component(object, parent);
    }
    else if (object.GetName() == "Divider")
    {
      return new Divider(object, parent);
    }
    else if (object.GetName() == "TextBox")
    {
      return new TextBox(object, parent);
    }
    else if(object.GetName() == "Container")
    {
      return new Container(object, parent);
    }
    Log::Warning("Could not read XML object ", object.GetName(), ".");
    XMLObject o;
    return new Component(o,nullptr); // Return plain content to avoid crash.
  }
    bool GUIUtils::GetBooleanFromXML(const XMLObject& object, const std::string& key, bool defaultValue)
    {
      if(object.HasProperty(key))
        return GetBoolean(object.GetProperty(key));
      return defaultValue;
    }

		Vec4 GUIUtils::GetColorFromXML(const XMLObject& object, const std::string& key, const Vec4& defaultValue)
    {
      if(object.HasProperty(key))
        return GetColor(object.GetProperty(key));
      return defaultValue;
    }

		float GUIUtils::GetSizeFromXML(const XMLObject& object, const std::string& key, float defaultValue, float parentSize)
    {
      if(object.HasProperty(key))
        return GetSize(object.GetProperty(key), parentSize);
      return defaultValue;
    }
		std::string GUIUtils::GetStringFromXML(const XMLObject& object, const std::string& key, const std::string& defaultValue)
    {
      if(object.HasProperty(key))
        return object.GetProperty(key);
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
