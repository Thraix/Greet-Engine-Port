#pragma once

#include <graphics/gui/TLBR.h>

namespace Greet
{

  class Component;

  struct Style
  {
    public:
      TLBR margin;
      TLBR padding;
      TLBR border;
      Vec4 backgroundColor;
      Vec4 borderColor;
      float borderRadius;
      float radius;
      uint roundedPrecision;

      Style()
        : margin(0,0,0,0), padding(0,0,0,0), border(0,0,0,0), backgroundColor(Vec4(1,1,1,1)), borderColor(Vec4(0,0,0,0)), borderRadius(0.0), radius(0.0), roundedPrecision(3)
      {

      }

      void Load(const std::string& prefix, const Component& content)
      {
        Load(prefix,content,nullptr);
      }

      void Load(const std::string& prefix, const Component& content, Style const * inherit);
  };
}
