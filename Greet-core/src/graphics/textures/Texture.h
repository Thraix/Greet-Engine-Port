#pragma once

#include <string>
#include <internal/GreetGL.h>
#include <internal/GreetTypes.h>

namespace Greet {

  enum class TextureFilter
  {
    NONE,
    LINEAR = GL_LINEAR,
    NEAREST = GL_NEAREST
  };

  enum class TextureWrap
  {
    NONE,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER,
    MIRRORED_REPEAT,
    REPEAT,
    MIRROR_CLAMP_TO_EDGE
  };

  enum class TextureInternalFormat
  {
    DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
    DEPTH_STENCIL = GL_DEPTH_STENCIL,
    RED = GL_RED,
    RGB = GL_RGB,
    RGBA = GL_RGBA
  };

  enum class TextureFormat
  {
    NORMAL,
    INVERTED,
  };

  struct TextureParams
  {
    TextureParams()
      : filter(TextureFilter::LINEAR), wrap(TextureWrap::CLAMP_TO_EDGE), internalFormat(TextureInternalFormat::RGBA),format(TextureFormat::INVERTED)
    {}

    TextureParams(TextureFilter filter, TextureWrap wrap, TextureInternalFormat internalFormat, TextureFormat format)
      : filter(filter), wrap(wrap), internalFormat(internalFormat), format(format)
    {}

    TextureFilter filter;
    TextureWrap wrap;
    TextureInternalFormat internalFormat;
    TextureFormat format;
  };

  class Texture
  {
    protected:
      uint m_texId;
      uint m_textureType;
    public:
      Texture(uint textureType, bool generateTexture = true);
      Texture(uint texId, uint textureType);
      Texture(const Texture& texture);
      Texture& operator=(const Texture& shader);
      virtual ~Texture();
      virtual void Enable() const;
      virtual void Disable() const;

      inline uint GetTexId() const { return m_texId; }
      friend bool operator<(const Texture& tex1, const Texture& tex2)
      {
        return tex1.m_texId < tex2.m_texId;
      }

    protected:
      inline void SetTexId(uint texId) { m_texId = texId; }
  };

}
