#pragma once

#include <graphics/Renderable.h>
#include <string>
#include <math/Maths.h>
#include <graphics/fonts/Font.h>
#include <internal/GreetTypes.h>
#include <graphics/fonts/FontManager.h>

namespace Greet {

	class Label : public Renderable
	{
		private:
			Font* m_font;
		
		public:
			std::string text;
			Vec2 position;
			uint color;
			
			
		public:
			Label(const std::string& text, Vec2 pos,Font* font, uint color);
			Label(const std::string& text, Vec2 pos,std::string fontname, uint fontsize, uint color);
			
			void Submit(Renderer2D* renderer) const override;
			bool Update(float timeElapsed) override;

			void SetColor(uint c) override { color = c; }
			inline uint GetColor() const override { return color;}
			void SetPosition(const Vec2& pos) override { position = pos; }
			inline const Vec2& GetPosition() const override { return position;}
			void SetSize(const Vec2& size) override {  }
			inline const Vec2& GetSize() const override { return Vec2(GetWidth(),m_font->GetSize());} // TODO: RETURN SIZE OF TEXT
			float GetWidth() const;
	};
}
