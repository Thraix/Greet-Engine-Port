#pragma once

#include <internal/GreetGL.h>
#include <graphics/textures/Texture2D.h>
#include <map>
#include <math/Vec4.h>
#include <logging/Logger.h>

namespace Greet
{
  struct Glyph
  {
    float width;
    float kerning;
    float advanceX;
    float advanceY;
    float height;
    float ascending;
    float descending;
    Vec4 textureCoords;
  };



  class FontAtlas
  {
    private:
      FT_Library library;
      FT_Face face;
      Texture2D atlas;
      uint width;
      uint height;
      uint fontSize;
      std::map<char, Glyph> glyphs;
      char* m_pixels;

      uint yPos;
      uint xPos;
      uint nextYPos;

    public:
      FontAtlas(const std::string& filename, uint width, uint height, uint fontSize);
      virtual ~FontAtlas();

      const Glyph& GetGlyph(char character);
      uint GetTextureId() const;
      uint GetBaselineOffset() const;
    private:
      const Glyph& AddGlyph(char character);
  };
}
