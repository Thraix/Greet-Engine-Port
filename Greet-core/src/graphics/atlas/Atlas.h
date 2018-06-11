#pragma once

#include <utils/ImageUtils.h>
#include <graphics/textures/Texture.h>
#include <graphics/Sprite.h>
#include <list>
#include <vector>

namespace Greet{
	class Atlas : public Texture2D
	{
	private:
		std::vector<std::string> m_textureNames;
		std::vector<uint> m_textureNamePos;
		std::vector<bool> m_occupied;

		uint m_textureSize;
		uint m_texturesSide;
		uint m_textures;

		BYTE* m_bits;
	public:
		Atlas(uint atlasSize, uint textureSize);
		virtual ~Atlas();

		bool AddTexture(std::string name, std::string filePath);
		void RemoveTexture(std::string textureName);
		void AddTexture(BYTE* bits, uint bpp, std::string name);
		Sprite* GetSprite(std::string name);
		Sprite* GetSpriteFromSheet(std::string sheetName, Vec2 texPos, Vec2 texSize);
	private:
		void FillTexture(uint x, uint y, BYTE* bits, uint bpp);
		void UpdateTexture();
	};
}
