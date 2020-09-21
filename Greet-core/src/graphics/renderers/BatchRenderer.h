#pragma once

#include <common/Types.h>

#include <graphics/Renderable2D.h>
#include <graphics/RenderablePoly.h>
#include <graphics/buffers/VertexArray.h>
#include <graphics/renderers/Renderer2D.h>

#include <vector>

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
      Ref<VertexBuffer> vbo;
      Ref<VertexArray> vao;
      Ref<Buffer> ibo;

      int maxTextures;
      uint m_iboSize;
      uint m_lastIndex;
      uint* indices;
      VertexData* m_buffer;

      std::vector<uint> m_texSlots;

    public:
      BatchRenderer(const Ref<Shader>& shader);
      virtual ~BatchRenderer();
      void Begin() override;
      void Draw(const Renderable2D& renderable) override;
#if 0
      void Submit(const RenderablePoly* renderable);
#endif
      void DrawRect(const Mat3& transform, uint texID, Vec2f texPos, Vec2f texSize, uint color, uint maskTexId, const Vec2f& maskTexPos, const Vec2f& maskTexSize);
      void DrawRect(const Vec2f& position, const Vec2f& size, uint texID, Vec2f texPos, Vec2f texSize, uint color, uint maskTexId, const Vec2f& maskTexPos, const Vec2f& maskTexSize);
      void DrawRect(const Vec2f& position, const Vec2f& size, uint texID, Vec2f texPos, Vec2f texSize, uint color);
      void DrawText(const std::string& text, const Vec2f& position, const Font& font, const uint& color);
      void Draw(const Mat3& transform, const Vec2f& texPos, const Vec2f& texSize, const uint textureSlot, const uint& color, const uint& maskTexSlot, const Vec2f& maskTexPos, const Vec2f& maskTexSize);
      void Draw(const Vec2f& position, const Vec2f& size, const Vec2f& texPos, const Vec2f& texSize, const uint textureSlot, const uint color, const uint& maskTexSlot, const Vec2f& maskTexPos, const Vec2f& maskTexSize);
      void Draw(const Vec2f& position, const Vec2f* vertices, const uint amount, const uint color);
      void DrawRect(const Vec2f& position, const Vec2f& size, const uint& color);
      void DrawLine(const Vec2f& pos1, const Vec2f& pos2, float width, uint color);
      void FillRect(const Vec2f& position, const Vec2f& size, const uint& color);
      void FillRect(const Vec2f& position, const Vec2f& size, const uint& color, const Sprite* mask);
      void AppendVertexBuffer(const Vec2f& position, const Vec2f& texCoord, const uint& texID, const uint& color, const uint& maskTexId, const Vec2f& maskTexCoord);
      uint GetTextureSlot(const uint texID);
      void End() override;
      void Flush() override;
      void EnableBuffers();
      void DisableBuffers();
      void AddIndicesPoly(uint poly);
      void SetShader(const Ref<Shader>& aShader) override;
    private:
      void Init();
  };
}
