#pragma once

#include <internal/GreetGL.h>
#include <internal/GreetTypes.h>
#include <graphics/textures/Texture.h>
#include <string>
#include <math/Maths.h>
#include <logging/Log.h>

namespace Greet{

	class FontContainer;

	class Font
	{
	private:
		FontContainer* m_container;
		ftgl::texture_atlas_t* m_atlas;
		ftgl::texture_font_t* m_font;
		float m_size;
		
	public:
		Font(FontContainer* container, uint size);
		void Init();
		inline ftgl::texture_font_t* GetFTFont() const { return m_font;}
		inline float GetSize() const {return m_size;}
		inline float GetAscender() const { return m_font->ascender; }
		inline float GetDescender() const { return m_font->descender; }
		inline uint GetAtlasID() const {return m_atlas->id;}
		float GetWidthOfText(const std::string& text, uint startPos, uint endPos) const;
		float GetWidthOfText(const std::string& text) const;
		float* GetPartialWidths(const std::string& text);

		friend bool operator<(const Font& f1, const Font& f2)
		{
			return f1.GetSize() < f2.GetSize();
		}
	};

	struct FontCompare
	{
		using is_transparent = void;

		// Yes these arguments are dumb, but atleast the compiler is not complaining anymore+-
		bool operator()(Font *const& f1, Font *const& f2) const
		{
			return f1->GetSize() < f2->GetSize();
		}

		// Yes these arguments are dumb, but atleast the compiler is not complaining anymore
		bool operator()(const uint& s, Font *const& f) const
		{
			return s < f->GetSize();
		}

		// Yes these arguments are dumb, but atleast the compiler is not complaining anymore
		bool operator()(Font *const& f, const uint& s) const
		{
			return f->GetSize() < s;
		}
	};
}
