#include "Material.h"

#include <graphics/textures/TextureManager.h>

namespace Greet {

  Material::Material(Shader&& shader, const Texture2D& texture)
    : m_shader{std::move(shader)}, m_texture(texture), m_color(0xffffffff)
  {
    UpdateTexture();
  }

  Material::Material(Shader&& shader)
    : m_shader(std::move(shader)), m_texture(TextureManager::GetEmptyTexture2D()), m_color(0xffffffff)
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
    m_texture.Enable();

  }

  void Material::Unbind() const
  {
    m_texture.Disable();
    m_shader.Disable();
  }

  void Material::SetShader(Shader&& shader)
  {
    m_shader = std::move(shader);
    UpdateTexture();
  }

  void Material::UpdateTexture()
  {
    m_shader.Enable();
    m_shader.SetUniformBoolean("hasTexture", m_texture.GetTexId() != 0);
    m_shader.Disable();
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
