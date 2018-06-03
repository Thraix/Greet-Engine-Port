#include "Material.h"

namespace Greet {

	Material::Material(const Shader& shader, Texture* texture)
		: m_shader(shader), m_texture(texture)
	{
		UpdateTexture();
	}

	Material::~Material()
	{
	}

	void Material::Bind() const
	{
		m_shader.Enable();
		m_shader.SetUniform1f("reflectivity", m_reflectivity);
		m_shader.SetUniform1f("shineDamper", m_shineDamper);
		m_shader.SetUniform4f("mat_color", ColorUtils::ColorHexToVec4(m_color));
		//m_shader.setUniform3f("fogColor", Vec3(0.0,1.0,0.0));
		if (m_texture != NULL)
			m_texture->Enable();
			
	}

	void Material::Unbind() const
	{
		if (m_texture != NULL)
			m_texture->Disable();
		m_shader.Disable();
	}

	void Material::SetShader(const Shader& shader)
	{
		m_shader = shader;
		UpdateTexture();
	}

	void Material::UpdateTexture()
	{
		if (m_texture == NULL)
		{
			m_shader.Enable();
			m_shader.SetUniformBoolean("hasTexture", false);
			m_shader.Disable();
		}
	}

	Material* Material::SetReflectivity(float reflectivity) 
	{
		m_reflectivity = reflectivity;
		return this;
	}

	Material* Material::SetShineDamper(float shineDamper)
	{
		m_shineDamper = shineDamper;
		return this;
	}
}
