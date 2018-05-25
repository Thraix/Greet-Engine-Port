#include "Label.h"

namespace Greet {
	
	Label::Label(const std::string& text, Vec2 pos,Font* font, uint color)
	:text(text),position(pos),m_font(font),color(color)
	{
		
	}
	
	Label::Label(const std::string& text, Vec2 pos,std::string fontname, uint fontsize, uint color)
	:text(text),position(pos),m_font(FontManager::Get(fontname,fontsize)),color(color)
	{
		
	}
	
	void Label::Submit(Renderer2D* renderer) const
	{
		renderer->SubmitString(text,position,m_font,color);
	}
	
	bool Label::Update(float timeElapsed)
	{
		return false;
	}

	float Label::GetWidth() const
	{
		float width = 0;
		uint length = text.length();
    const char* str = text.c_str();
		for (uint i = 0;i < length;i++)
		{
			ftgl::texture_glyph_t* glyph = ftgl::texture_font_get_glyph(m_font->GetFTFont(), str + i);
			if (glyph != NULL)
			{
				if (i != length - 1)
					width += glyph->advance_x;
				else
					width += glyph->width;
			}
		}
		return width;
	}
	
}
