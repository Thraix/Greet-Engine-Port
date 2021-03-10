#include "FrameBufferObject.h"

#include <graphics/Window.h>
#include <internal/GreetGL.h>
#include <math/Maths.h>

namespace Greet {

  FrameBufferObject::FrameBufferObject(uint width, uint height)
    : width(width), height(height)
  {
    GLCall(glGenFramebuffers(1, &fbo));

    // Color buffer
    //attachDepthTexture(); AttachColorTexture(GL_COLOR_ATTACHMENT0);

    //return;
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER,fbo));
    GLCall(glGenRenderbuffers(1,&depthBuffer));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER,depthBuffer));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,width,height));

    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthBuffer));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
  }

  FrameBufferObject::~FrameBufferObject()
  {
    GLCall(glDeleteFramebuffers(1, &fbo));
    GLCall(glDeleteBuffers(1,&depthBuffer));
    colorTextures.clear();
  }

  void FrameBufferObject::AttachColorTexture(uint attachmentId)
  {
    if (attachmentId < GL_COLOR_ATTACHMENT0 || attachmentId > GL_COLOR_ATTACHMENT15)
    {
      Log::Error("Color attachment is out of range: ",attachmentId);
      return;
    }
    if (colorTextures.find(attachmentId) != colorTextures.end())
    {
      Log::Error("The given attachment is already in use: ", attachmentId);
    }
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
    Ref<Texture2D> colorTexture = Texture2D::Create(width,height,TextureParams(TextureFilter::LINEAR,TextureWrap::NONE,TextureInternalFormat::RGB));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentId, GL_TEXTURE_2D, colorTexture->GetTexId(), 0));
    colorTextures.emplace(attachmentId,colorTexture);
    uint size = colorTextures.size();
    uint* colorBuffers = new uint[size];
    uint i = 0;
    for (auto it = colorTextures.begin();it != colorTextures.end();it++)
      colorBuffers[i++] = it->first;
    GLCall(glDrawBuffers(size,colorBuffers));
    delete[] colorBuffers;
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER,0));
  }

  void FrameBufferObject::AttachDepthTexture()
  {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
    depthTexture = Texture2D::Create(width,height,TextureParams(TextureFilter::NEAREST,TextureWrap::NONE,TextureInternalFormat::DEPTH_COMPONENT));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->GetTexId(), 0));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER,0));
  }

  void FrameBufferObject::Bind()
  {
    GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo));
    GLCall(glClearColor(0,0,0,0));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GLCall(glViewport(0,0,width,height));
  }

  void FrameBufferObject::Unbind()
  {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GLCall(glViewport(0,0,Window::GetWidth(),Window::GetHeight()));
  }

  const Ref<Texture2D>& FrameBufferObject::GetColorTexture(uint attachmentId) const
  {
    auto it = colorTextures.find(attachmentId);
    if (it != colorTextures.end())
      return it->second;
    Log::Error("There is no Texture with the given attachmentId: ", attachmentId);
    return colorTextures.begin()->second;
  }
}
