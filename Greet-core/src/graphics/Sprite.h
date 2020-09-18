#pragma once

#include <common/Types.h>
#include <math/Maths.h>
#include <graphics/textures/Texture2D.h>
#include <graphics/textures/TextureManager.h>

namespace Greet{
  class Sprite
  {
    protected:
      Ref<Texture2D> texture;
      Vec2f texPos;
      Vec2f texSize;
    public:
      Sprite()
        : texture(nullptr), texPos(Vec2f(0, 0)), texSize(Vec2f(1, 1))
      {

      }

      virtual bool Update(float timeElapsed) { return false; }
      Sprite(const Ref<Texture2D>& texture)
        : texture{texture}, texPos{Vec2f{0, 0}}, texSize{Vec2f{1, 1}}
      {}

      Sprite(const Ref<Texture2D>& texture, const Vec2f& texPos, const Vec2f& texSize)
        : texture{texture}, texPos{texPos}, texSize{texSize}
      {}

      virtual ~Sprite() {}

      inline uint GetTextureID() const { return texture->GetTexId(); }

      inline Sprite* FromSpriteSheet(const Vec2f& texPos, const Vec2f& texSize) const
      {
        return new Sprite(texture, texPos + texPos * texSize, texSize * texSize);
      }

      virtual inline const Vec2f& GetTexPos() const { return texPos; }
      virtual inline const Vec2f& GetTexSize() const { return texSize; }
  };
}
