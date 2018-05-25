#include "Font.h"

namespace Greet{

	Font::Font(FontContainer* container, uint size)
	: m_container(container), m_size(size)
	{
		Init();
	}

	void Font::Init()
	{
		m_atlas = ftgl::texture_atlas_new(512,512,4);
		if (m_container->GetData() == NULL)
		{
			m_font = ftgl::texture_font_new_from_file(m_atlas, m_size, m_container->GetFileName().c_str());
			ASSERT(m_font, "Could not load font from file: ", m_container->m_filename);
		}
		else
		{
			m_font = ftgl::texture_font_new_from_memory(m_atlas, m_size, m_container->m_data, m_container->m_datasize);
			ASSERT(m_font, "Could not load font from memory:", m_container->m_filename);
		}
	
	}
	
	float Font::GetWidthOfText(const std::string& text, uint startPos, uint endPos) const
	{
		float width = 0;
		if (startPos > text.size() || endPos < startPos || endPos > text.size())
		{
			Log::Error("Invalid start and endpos.");
			return 0;
		}

		for (uint i = startPos;i < endPos;i++)
		{
			const char& c = text[i];
			ftgl::texture_glyph_t* glyph = ftgl::texture_font_get_glyph(GetFTFont(), text.c_str()+i);
			if (glyph != NULL)
			{
				width += glyph->advance_x;
			}
		}
		return width;
	}

	float Font::GetWidthOfText(const std::string& text) const
	{
		return GetWidthOfText(text,0,text.size());
	}

	float* Font::GetPartialWidths(const std::string& text)
	{
		float width = 0;

		float* widths = new float[text.size()+1];

		for (uint i = 0;i < text.size();i++)
		{
			const char& c = text[i+1];
			ftgl::texture_glyph_t* glyph = ftgl::texture_font_get_glyph(GetFTFont(), text.c_str()+i);
			if (glyph != NULL)
			{
				widths[i] = width;
				if (i != text.size() - 1 || c == ' ')
					width += glyph->advance_x;
				else
					width += glyph->width;
			}
		}
		widths[text.size()] = width;
		return widths;
	}
}
