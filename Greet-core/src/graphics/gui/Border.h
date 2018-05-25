#pragma once



// Coordinates in the texture file
//
//                               u3
//  |----------------------------|
//  |                         u2 |
//  |-------------------------|  |
//  |      u1                 |  |
//  |------|                  |  |______________________
//  |   u0 |                  |  |  |      |     |     |
//  |---|  |                  |  |  |      |     |     |
//  |   | _|__________________|_ |__|_ v0  |     |     |
//  |    /                      \          |     |     |
//  |   /   __________________   \_________|_ v1 |     |
//  |   |  |                  |  |               |     |
//  |   |  |                  |  |               |     |
//  |   |  |                  |  |               |     |
//  |   |  |                  |  |               |     |
//  |   |  |                  |  |               |     |
//  |   |  |                  |  |               |     |
//  |   |  |                  |  |               |     |
//  |   |  |__________________|  |_______________|_ v2 |
//  |   \                        /                     |
//  |    \______________________/______________________|_ v3




#include <math/Maths.h>
#include <graphics/Renderable.h>
#include <internal/GreetTypes.h>
#include <graphics/textures/TextureManager.h>

namespace Greet{
	struct BorderData
	{
		Texture* texture;
		uint borderSize;
		Vec2 uv0;
		Vec2 uv1;
		Vec2 uv2;
		Vec2 uv3;

		BorderData(const std::string& texturename, const uint& borderSize, const Vec2& uv0, const Vec2& uv1, const Vec2& uv2, const Vec2& uv3)
		: texture(TextureManager::Get(texturename)), borderSize(borderSize), uv0(uv0), uv1(uv1), uv2(uv2), uv3(uv3)
		{

		}
		~BorderData()
		{

		}

		static BorderData* getDefaultBorderData()
		{
			return new BorderData("frame",5,Vec2(0.0f,0.0f),Vec2(5.0f/16.0f,5.0f/16.0f),Vec2(11.0f/16.0f,11.0f/16.0f),Vec2(1.0f,1.0f));
		}
	};

	class Border : public Renderable
	{
	private:
		Vec2 m_size;
		Vec2 m_position;
		BorderData* data;
		uint m_color;
	public:
		Border(const Vec2& position,const Vec2& size, const std::string& texturename, const uint& borderSize, const Vec2& uv0, const Vec2& uv1, const Vec2& uv2, const Vec2& uv3);
		Border(const Vec2& position,const Vec2& size, BorderData* data);
		virtual ~Border();

		void Submit(Renderer2D* renderer) const override;
		bool Update(float timeElapsed) override;
		void SetPosition(const Vec2& position) override;
		inline const Vec2& GetPosition() const override {return m_position; }
		void SetSize(const Vec2& size);
		inline const Vec2& GetSize() const override {return m_size;}
		void SetColor(uint color) override { m_color = color; }
		uint GetColor() const override { return m_color; }
	};

}
