#pragma once

#include <string>
#include <ctime>
#include <internal/GreetTypes.h>
#include <internal/GreetGL.h>

#define BIT(x) (1 << (x))

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
}}
