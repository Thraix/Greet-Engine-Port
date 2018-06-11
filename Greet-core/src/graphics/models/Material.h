#pragma once

#include <graphics/shaders/Uniformable.h>
#include <graphics/shaders/Shader.h>
#include <graphics/textures/Texture.h>
#include <utils/ColorUtils.h>

namespace Greet {

	class Material// : public Uniformable
	{
	private:
		Shader m_shader;
		Texture m_texture;
		float m_shineDamper = 10;
		float m_reflectivity = 1;
		uint m_color;

	public:
		Material(const Shader& shader, const Texture& texture);
    Material();
		virtual ~Material();

		void Bind() const;
		void Unbind() const;
		void SetShader(const Shader& shader);
		Material* SetReflectivity(float reflectivity);
		Material* SetShineDamper(float shineDamper);
		Material* SetColor(uint color) { m_color = color; return this; }

		inline float GetReflectivity() const { return m_reflectivity; }
		inline float GetShineDamper() const { return m_shineDamper; }
		inline uint GetColor() const { return m_color; }
		inline const Shader& GetShader() const { return m_shader; }
		inline const Texture& GetTexture() const { return m_texture; }
		inline uint GetTextureId() const { return m_texture.GetTexId(); }
	private:
		void UpdateTexture();
	};

}
