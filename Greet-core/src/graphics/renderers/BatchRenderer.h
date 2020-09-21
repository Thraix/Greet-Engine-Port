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
      void DrawRect(const Mat3& transform, uint color = 0xffffffff);
      void DrawRect(const Mat3& transform, const Ref<Texture2D>& texture, const Vec2f& texPos = Vec2f{0, 0}, const Vec2f& texSize = Vec2f{1, 1}, uint color = 0xffffffff);

      void DrawRect(const Vec2f& position, const Vec2f& size, uint color = 0xffffffff);
      void DrawRect(const Vec2f& position, const Vec2f& size, const Ref<Texture2D>& texture, const Vec2f& texPos = Vec2f{0, 0}, const Vec2f& texSize = Vec2f{1, 1}, uint color = 0xffffffff);

      void DrawText(const std::string& text, const Vec2f& position, const Font& font, uint color = 0xffffffff);
      void Draw(const Vec2f& position, const Vec2f* vertices, uint amount, uint color = 0xffffffff);
      void DrawLine(const Vec2f& pos1, const Vec2f& pos2, float width, uint color = 0xffffffff);
      void AppendVertexBuffer(const Vec2f& position, const Vec2f& texCoord, uint texSlot, uint color);
      uint GetTextureSlot(const Ref<Texture2D>& texture);
      uint GetTextureSlot(uint texID);
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
