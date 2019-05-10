#pragma once

#include <graphics/renderers/Renderer2D.h>
#include <internal/GreetTypes.h>

#include <graphics/Renderable2D.h>
#include <graphics/RenderablePoly.h>
#include <graphics/buffers/BatchIndexBuffer.h>

#define RENDERER_MAX_TEXTURES	32
#define RENDERER_MAX_SPRITES	360000
#define RENDERER_VERTEX_SIZE	sizeof(VertexData)
#define RENDERER_SPRITE_SIZE	RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE	RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE	RENDERER_MAX_SPRITES * 6

#define SHADER_VERTEX_INDEX			0x00
#define SHADER_TEXCOORD_INDEX		0x01
#define SHADER_TEXID_INDEX			0x02
#define SHADER_COLOR_INDEX			0x03
#define SHADER_MASK_TEXCOORD_INDEX	0x04
#define SHADER_MASK_TEXID_INDEX		0x05

namespace Greet {

  class BatchRenderer : public Renderer2D
  {

    private:
      uint m_ibo;
      uint m_vbo;
      uint m_vao;
      uint m_iboSize;
      uint m_lastIndex;
      uint* m_indices;
      VertexData* m_buffer;

      std::vector<GLuint> m_texSlots;

    public:
      BatchRenderer();
      virtual ~BatchRenderer();
      void Begin() override;
      void Submit(const Renderable2D* renderable) override;
#if 0
      void Submit(const RenderablePoly* renderable);
#endif
      void Submit(const Transform& transform, uint texID, Vec2 texPos, Vec2 texSize, uint color, uint maskTexId, const Vec2& maskTexPos, const Vec2& maskTexSize);
      void Submit(const Vec2& position, const Vec2& size, uint texID, Vec2 texPos, Vec2 texSize, uint color, uint maskTexId, const Vec2& maskTexPos, const Vec2& maskTexSize);
      void Submit(const Vec2& position, const Vec2& size, uint texID, Vec2 texPos, Vec2 texSize, uint color);
      void SubmitString(const std::string& text, const Vec2& position, Font* font, const uint& color);
      inline void Draw(const Transform& transform, const Vec2& texPos, const Vec2& texSize, const uint textureSlot, const uint& color, const uint& maskTexSlot, const Vec2& maskTexPos, const Vec2& maskTexSize);
      inline void Draw(const Vec2& position, const Vec2& size, const Vec2& texPos, const Vec2& texSize, const uint textureSlot, const uint color, const uint& maskTexSlot, const Vec2& maskTexPos, const Vec2& maskTexSize);
      inline void Draw(const Vec2& position, const Vec2* vertices, const uint amount, const uint color);
      inline void DrawRect(const Vec2& position, const Vec2& size, const uint& color);
      inline void FillRect(const Vec2& position, const Vec2& size, const uint& color);
      inline void FillRect(const Vec2& position, const Vec2& size, const uint& color, const Sprite* mask);
      inline void AppendVertexBuffer(const Vec2& position, const Vec2& texCoord, const uint& texID, const uint& color, const uint& maskTexId, const Vec2& maskTexCoord);
      uint GetTextureSlot(const GLuint texID);
      void End() override;
      void Flush() override;
      void EnableBuffers();
      void DisableBuffers();
      void AddIndicesPoly(uint poly);
    private:
      void Init();
  };
}
