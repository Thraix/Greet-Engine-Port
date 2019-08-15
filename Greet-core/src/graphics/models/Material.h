#pragma once

#include <graphics/shaders/Uniformable.h>
#include <graphics/shaders/Shader.h>
#include <graphics/shaders/ShaderFactory.h>
#include <graphics/textures/Texture2D.h>
#include <utils/ColorUtils.h>
#include <graphics/cameras/Camera.h>

namespace Greet {

  class Material
  {
    private:
      Shader m_shader;
      uint uuid;
      const Texture2D& m_texture;
      float specularExponent = 10;
      float specularStrength = 0.5;
      float diffuseStrength = 0.5;
      Vec4 color;

    public:
      Material(Shader&& shader, const Texture2D& texture);
      Material(Shader&& shader);
      virtual ~Material();

      void Bind(const Camera* camera) const;
      void Unbind() const;
      void SetShader(Shader&& shader);
      Material* SetSpecularExponent(float specularExponent) { this->specularExponent = specularExponent; return this; }
      Material* SetSpecularStrength(float specularStrength) { this->specularStrength = specularStrength; return this; }
      Material* SetDiffuseStrength(float diffuseStrength) {this->diffuseStrength = diffuseStrength; return this;}
      Material* SetColor(const Vec4& color) { this->color = color; return this; }

      inline float GetSpecularExponent() const { return specularExponent; }
      inline float GetSpecularStrength() const { return specularStrength; }
      inline float GetDiffuseStrength() const { return diffuseStrength; }
      inline const Vec4& GetColor() const { return color; }
      inline const Shader& GetShader() const { return m_shader; }
      inline const Texture& GetTexture() const { return m_texture; }
      inline uint GetTextureId() const { return m_texture.GetTexId(); }
      friend bool operator<(const Material& m1, const Material& m2) { return m1.uuid < m2.uuid;}
    private:
      void UpdateTexture();
  };

}
