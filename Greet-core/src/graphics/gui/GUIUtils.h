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

		static Frame* GetFrame(const XMLObject& object);
		static Component* GetComponent(const XMLObject& object, Component* parent);

		static bool GetBoolean(const std::string& str)
		{
			if (str == "true")
				return true;
			return false;
		}

		static Vec4 GetColor(const std::string& str)
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

		static bool IsPercentageSize(const std::string& size)
		{
			return StringUtils::ends_with(size, "%");
		}

		static bool IsStaticSize(const std::string& size)
		{
			return !StringUtils::ends_with(size, "%") || StringUtils::ends_with(size, "s%");
		}

		static float CalcSize(const std::string& size, float parentSize)
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
	};
}
