#pragma once

#include <string>
#include <internal/GreetGL.h>
#include <memory>
#include <common/Types.h>

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

  struct TextureParams
  {
    TextureParams()
      : filter(TextureFilter::LINEAR), wrap(TextureWrap::CLAMP_TO_EDGE), internalFormat(TextureInternalFormat::RGBA)
    {}

    TextureParams(TextureFilter filter, TextureWrap wrap, TextureInternalFormat internalFormat)
      : filter(filter), wrap(wrap), internalFormat(internalFormat)
    {}

    TextureFilter filter;
    TextureWrap wrap;
    TextureInternalFormat internalFormat;
  };

  
  struct TextureDeleter final
  {
    void operator()(uint* id);
  };

  class Texture
  {
    protected:
      std::unique_ptr<uint,TextureDeleter> texId; 
      uint m_textureType;
    public:
      Texture(uint textureType, bool generateTexture = true);
      Texture(uint texId, uint textureType);
      Texture();

      Texture(Texture&& texture) = default;
      Texture& operator=(Texture&& texture) = default;

      virtual void Enable() const;
      virtual void Disable() const;

      uint GetTexId() const;
      bool Invalid() const { return texId.get() == 0;};
      friend bool operator<(const Texture& tex1, const Texture& tex2)
      {
        return *tex1.texId< *tex2.texId;
      }
      friend bool operator==(const Texture& tex1, const Texture& tex2)
      {
        return *tex1.texId == *tex2.texId;
      }
  };

}
