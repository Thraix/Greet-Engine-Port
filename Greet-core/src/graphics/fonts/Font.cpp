#include "Font.h"
#include <graphics/fonts/FontContainer.h>

namespace Greet{

  Font::Font(FontContainer* container, uint size)
    : m_container(container), m_size(size)
  {
    Init();
  }
  Font::~Font()
  {
    delete m_atlas;
  }

  void Font::Init()
  {
    m_atlas = new FontAtlas(m_container->GetFileName(),512,512, m_size);
    /*
       if (m_container->GetData() == NULL)
       {
       m_font = ftgl::texture_font_new_from_file(m_atlas, m_size, m_container->GetFileName().c_str());
       Log::Info("Here");
       ASSERT(m_font, "Could not load font from file: ", m_container->m_filename);
       }
       else
       {
       m_font = ftgl::texture_font_new_from_memory(m_atlas, m_size, m_container->m_data, m_container->m_datasize);
       ASSERT(m_font, "Could not load font from memory:", m_container->m_filename);
       }
       */
  }

  float Font::GetWidthOfText(const std::string& text, uint startPos, uint endPos, float scale) const
  {
    float width = 0;
    if (startPos > text.size() || endPos < startPos || endPos > text.size())
    {
      Log::Error("Invalid start and endpos.");
      return 0;
    }

    for (uint i = startPos;i < endPos;i++)
    {
      const Glyph& glyph = m_atlas->GetGlyph(text[i]);
      // If it is the last char do not include the advancment
      if(i == endPos - 1 && text[i] != ' ')
        width += glyph.width;
      else
        width += glyph.advanceX - glyph.kerning;
    }
    return width*scale;
  }

  float Font::GetWidthOfText(const std::string& text, float scale) const
  {
    return GetWidthOfText(text,0,text.size(),scale);
  }

  float* Font::GetPartialWidths(const std::string& text, float scale)
  {
    float width = 0;

    float* widths = new float[text.size()+1];

    for (uint i = 0;i < text.size();i++)
    {
      const Glyph& glyph = m_atlas->GetGlyph(text[i]);
      widths[i] = width*scale;
      // If it is the last char do not include the advancment
      if(i == text.size()- 1)
        width += glyph.width;
      else
        width += glyph.advanceX;
    }
    widths[text.size()] = width;
    return widths;
  }
}
