#pragma once

#include <unordered_map>

#include <graphics/fonts/Font.h>
#include <graphics/fonts/FontAtlas.h>
#include <graphics/fonts/FontContainer.h>

namespace Greet {
  class FontManager
  {
    private:
      static std::unordered_map<std::string, FontContainer> mvFonts;
    public:

      static bool Add(const std::string& asFontname, const FontContainer& aFont);
      static Font Get(const std::string& asFontname, uint aiSize);
      static void Destroy();
    private:
      FontManager(){}
  };
}
