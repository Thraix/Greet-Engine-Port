#pragma once

#include <internal/GreetGL.h>
#include <internal/GreetTypes.h>
#include <graphics/textures/Texture.h>
#include <string>
#include <math/Maths.h>
#include <logging/Log.h>
#include <graphics/fonts/FontContainer.h>

namespace Greet{


	class Font
	{
	private:
		FontContainer* m_container;
		ftgl::texture_atlas_t* m_atlas;
		ftgl::texture_font_t* m_font;
		float m_size;
		
	public:
		Font(FontContainer* container, uint size);
		Font(FontContainer* container, uint size, Vec2 scale);
		void Init();
		inline ftgl::texture_font_t* GetFTFont() const { return m_font;}
		inline float GetSize() const {return m_size;}
		inline uint GetAtlasID() const {return m_atlas->id;}
		float GetWidthOfText(const std::string& text, uint startPos, uint endPos) const;
		float GetWidthOfText(const std::string& text) const;
		float* GetPartialWidths(const std::string& text);
	};
}
