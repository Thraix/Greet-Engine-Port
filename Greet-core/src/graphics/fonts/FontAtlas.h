#pragma once

#include <graphics/textures/Texture2D.h>
#include <map>
#include <math/Vec4.h>
#include <logging/Logger.h>
#include <common/Memory.h>

typedef struct FT_LibraryRec_  *FT_Library;
typedef struct FT_FaceRec_*  FT_Face;

namespace Greet
{
  struct Glyph
  {
    float miWidth;
    float miBearingX;
    float miAdvanceX;
    float miAdvanceY;
    float miHeight;
    float miBearingY;
    float miDescending;
    Vec4f mvTextureCoords;
  };

  class FontAtlas final
  {
    private:
      FT_Library mLibrary;
      FT_Face mFace;
      Ref<Texture2D> mAtlas;
      uint miWidth;
      uint miHeight;
      std::map<char, Glyph> mvGlyphs;
      std::vector<byte> mvPixels;

      uint miYPos;
      uint miXPos;
      uint miNextYPos;

      uint miBaselineOffset;
      uint miMedianOffset;
      uint miMedianHeight;

    private:
      FontAtlas(const std::string& asFilename, uint aiFontSize);

    public:
      ~FontAtlas();

      FontAtlas(const FontAtlas&) = delete;
      FontAtlas& operator=(const FontAtlas&) = delete;

      const Glyph& GetGlyph(char acCharacter);
      uint GetTextureId() const;
      uint GetBaselineOffset() const;
      uint GetMedianOffset() const;
      uint GetMedianHeight() const;

      static Ref<FontAtlas> Create(const std::string& asFontname, uint aiFontSize);
    private:
      const Glyph& AddGlyph(char character);
  };
}
