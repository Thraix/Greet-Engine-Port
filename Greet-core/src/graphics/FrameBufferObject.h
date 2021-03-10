#pragma once

#include <common/Types.h>
#include <graphics/textures/Texture2D.h>
#include <map>

namespace Greet {

  class FrameBufferObject
  {
    private:
      uint fbo;
      uint width;
      uint height;
      std::map<uint,Ref<Texture2D>> colorTextures;
      Ref<Texture2D> depthTexture;
      uint depthBuffer;
    public:
      FrameBufferObject(uint width, uint height);
      virtual ~FrameBufferObject();
      void Bind();
      void Unbind();
      void AttachColorTexture(uint attachementId);
      void AttachDepthTexture();
      const Ref<Texture2D>& GetColorTexture(uint attachmentId) const;
      const Ref<Texture2D>& GetDepthTexture() const { return depthTexture; }
    private:
  };
}
