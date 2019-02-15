#include "Material.h"

#include <graphics/textures/TextureManager.h>
#include <utils/UUID.h>

namespace Greet {

  Material::Material(Shader&& shader, const Texture2D& texture)
    : m_shader{std::move(shader)}, m_texture(texture), color(Vec4(1,1,1,1)), uuid{UUID::GetInstance().GetUUID()}
  {
    UpdateTexture();
  }

  Material::Material(Shader&& shader)
    : m_shader(std::move(shader)), m_texture(TextureManager::GetEmptyTexture2D()), color(Vec4(1,1,1,1))
  {
    UpdateTexture();
  }

  Material::~Material()
  {
  }

  void Material::Bind(const Camera* camera) const
  {
    m_shader.Enable();
    m_shader.SetUniform1f("specularStrength", specularStrength);
    m_shader.SetUniform1f("specularExponent", specularExponent);
    m_shader.SetUniform1f("diffuseStrength", diffuseStrength);
    m_shader.SetUniform4f("mat_color", color);
    m_shader.SetUniformMat4("projectionMatrix", camera->GetProjectionMatrix());
    m_shader.SetUniformMat4("viewMatrix", camera->GetViewMatrix());
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
}
