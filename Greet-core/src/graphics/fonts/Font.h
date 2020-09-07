#pragma once

#include <common/Types.h>
#include <string>
#include <graphics/fonts/FontAtlas.h>

namespace Greet{

  class FontContainer;

  class Font final
  {
    private:
      Ref<FontAtlas> mAtlas;
      FontContainer* mContainer;
      uint mfSize;

    public:
      Font(const Ref<FontAtlas>& aAtlas, FontContainer* aContainer, uint aiSize);

      const Ref<FontAtlas>& GetFontAtlas() const;
      uint GetFontAtlasId() const;


      const std::string& GetName() const;

      uint GetSize() const;
      uint GetBaselineOffset() const;
      uint GetMedianOffset() const;
      uint GetMedianHeight() const;

      uint GetWidthOfText(const std::string& asText, uint startPos, uint endPos) const;
      uint GetWidthOfText(const std::string& asText) const;
      uint GetWidthOfText(const std::string_view& asText) const;

      std::vector<uint> GetPartialWidths(const std::string& asText) const;
  };
}
