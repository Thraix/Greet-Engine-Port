#pragma once

#include <graphics/textures/Texture.h>
#include <utils/ImageUtils.h>
#include <utils/HotSwapping.h>

namespace Greet {

  class Texture2D : public Texture, public Resource
  {
    public:
      friend class FrameBufferObject;
    protected:
      uint m_width;
      uint m_height;
      TextureParams m_params;

    public:
      Texture2D(uint width, uint height, TextureParams params = TextureParams());
      Texture2D(const std::string& filename, TextureParams params = TextureParams());
      Texture2D(const std::vector<BYTE>& bits, uint width, uint height, TextureParams params = TextureParams());
      Texture2D(uint texID, uint width, uint height, TextureParams params = TextureParams());
      Texture2D(TextureParams params = TextureParams());

      Texture2D(Texture2D&&) = default;
      Texture2D& operator=(Texture2D&&) = default;

      virtual ~Texture2D();

      void SetPixels(const std::vector<BYTE>& pixels) const;
      void ReloadResource() override;
      inline uint GetWidth() const { return m_width; }
      inline uint GetHeight() const { return m_height; }
    protected:
      void LoadTexture(const std::string& filename);
      void GenTexture(uint width, uint height);
      void GenTexture(const std::vector<BYTE>& bits);

  };
}
