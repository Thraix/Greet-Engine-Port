#pragma once

#include <string>
#include <ctime>
#include <internal/GreetTypes.h>
#include <internal/GreetGL.h>

#define BIT(x) (1 << x)

namespace Greet { namespace Utils {

	inline const std::string CurrentDateTime()
	{
		time_t now = time(0);
		struct tm tstruct;
		char buf[80];
		tstruct = *localtime(&now);
		strftime(buf,sizeof(buf),"%Y%m%d%H%M%S",&tstruct);
		return buf;
	}

	inline void Screenshot(uint width, uint height)
	{
		BYTE* pixels = new BYTE[3 * width*height];
		glReadPixels(0,0,width,height, GL_BGR, GL_UNSIGNED_BYTE, pixels);
		FIBITMAP* Image = FreeImage_ConvertFromRawBits(pixels, width, height, 3*width, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
		std::string s = "screenshot_";
		s += CurrentDateTime();
		s += ".png";
		FreeImage_Save(FIF_PNG, Image, s.c_str(), 0);
		delete[] pixels;
	}

	template<typename Base, typename T>
	inline bool InstanceOf(const T *ptr) {
		return dynamic_cast<const Base*>(ptr) != nullptr;
	}

	inline std::string GetTime()
	{
		time_t currentTime;
		struct tm *localTime;
		time(&currentTime);
		localTime = localtime(&currentTime);
		int hour = localTime->tm_hour;
		int min = localTime->tm_min;
		std::string res = "";
		if (hour < 10)
			res += "0";
		res += std::to_string(hour)+":";
		if (min < 10)
			res += "0";
		res += std::to_string(min);
		return res;
	}

	inline byte* GetBytesFromString(const std::string& s)
	{
		byte* bytes = new byte[s.length()];
		for(uint i = 0;i<s.length();i++)
		{
			bytes[i] = s[i];
		}
		return bytes;
	}

	inline uint HSVtoARGB(float hue, float sat, float val)
	{
		int h = (int)(hue * 6.0f);
		float f = hue * 6.0f - h;
		float p = val* (1.0f - sat);
		float q = val* (1.0f - f*sat);
		float t = val* (1.0f - (1.0f - f)*sat);
		float r = 0;
		float g = 0;
		float b = 0;
		switch (h) {
		case 0:
			r = val;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = val;
			b = p;
			break;
		case 2:
			r = p;
			g = val;
			b = t;
			break;
		case 3:
			r = p;
			g = q;
			b = val;
			break;
		case 4:
			r = t;
			g = p;
			b = val;
			break;
		case 5:
			r = val;
			g = p;
			b = q;
			break;
		}
		return 0xff000000 + ((int)(r * 255.0f) << 16) + ((int)(g * 255.0f) << 8) + (int)(b * 255.0f);
	}

	inline unsigned long long powi(uint b, uint n)
	{
		unsigned long long r = 1;
		for (uint i = 0; i < n; i++)
		{
			r *= b;
		}
		return r;
	}

	inline uint Hex10ToDec16(const char c)
	{
		if (c >= '0' && c <= '9')
			return (uint)(c - '0');
		else if (c >= 'a' && c <= 'f')
			return (uint)(c - 'a');
		Log::Warning("Utils::Hex10ToDec16 couldn't recognize the hex character \'", c, "\'");
		return 0;

		switch (c)
		{
		case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
			return (uint)(c - '0');
		case 'a':
			return 10;
		case 'b':
			return 11;
		case 'c':
			return 12;
		case 'd':
			return 13;
		case 'e':
			return 14;
		case 'f':
			return 15;
		default:
			return 0;
		}
	}

	inline char Dec16ToHex10(int dec)
	{
		switch (dec)
		{
		case 0:
			return '0';
		case 1:
			return '1';
		case 2:
			return '2';
		case 3:
			return '3';
		case 4:
			return '4';
		case 5:
			return '5';
		case 6:
			return '6';
		case 7:
			return '7';
		case 8:
			return '8';
		case 9:
			return '9';
		case 10:
			return 'a';
		case 11:
			return 'b';
		case 12:
			return 'c';
		case 13:
			return 'd';
		case 14:
			return 'e';
		case 15:
			return 'f';
		default:
			return '0';
		}
	}

	inline std::string DecToHex(uint dec, uint values = 1)
	{
		std::string s = "";
		bool begin = true;
		for (uint i = 8; i >= 1; i--)
		{
			char hex = Dec16ToHex10(((dec >> (i * 4 - 4)) & 0xf));
			if (hex != '0' || !begin || values >= i)
				s += hex;
			else if (hex != '0')
				begin = false;
		}
		return s;
	}

	inline uint HexToDec(std::string hex)
	{
		if (hex.length() > 8)
		{
			return 0;
		}
		else
		{
			uint result = 0;

			int size = hex.length();
			for (int i = 0; i < size; i++)
			{
				result += Hex10ToDec16(hex[i]) << ((size - i) * 4 - 4);
			}
			return result;
		}
	}
}}
