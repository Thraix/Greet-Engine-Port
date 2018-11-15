#pragma once

#include <graphics/gui/TLBR.h>

namespace Greet
{

  class Content;

  struct Style
  {
    public:
      TLBR margin;
      TLBR padding;
      TLBR border;
      Vec4 backgroundColor;
      Vec4 borderColor;

      Style()
        :margin(0,0,0,0), padding(10,10,10,10), border(0,0,0,0), backgroundColor(Vec4(1,1,1,1)), borderColor(Vec4(0,0,0,0))
      {

      }

      void Load(const std::string& prefix, const Content& content)
      {
        Load(prefix,content,nullptr);
      }

      void Load(const std::string& prefix, const Content& content, Style const * inherit);
  };
}
