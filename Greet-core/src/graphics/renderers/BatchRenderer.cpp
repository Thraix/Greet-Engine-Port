#include "BatchRenderer.h"

#include <graphics/RenderCommand.h>
#include <internal/GreetGL.h>
#include <logging/Log.h>

namespace Greet {

  BatchRenderer::BatchRenderer(const Ref<Shader>& shader)
    : Renderer2D{shader}
  {
    // TODO: Make this into a RenderCommand.
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextures);

    std::vector<int> samplerUnits(maxTextures);
    std::iota(samplerUnits.begin(), samplerUnits.end(), 0);
    shader->Enable();
    shader->SetUniform1iv("textures", maxTextures, samplerUnits.data());
    vao = VertexArray::Create();
    vbo = VertexBuffer::CreateDynamic(nullptr, RENDERER_BUFFER_SIZE);
    vbo->SetStructure({
        { shader->GetAttributeLocation("position"), BufferAttributeType::VEC2},
        { shader->GetAttributeLocation("texCoord"), BufferAttributeType::VEC2},
        { shader->GetAttributeLocation("texID"), BufferAttributeType::FLOAT},
        { shader->GetAttributeLocation("color"), BufferAttributeType::UBYTE4, true},
        { shader->GetAttributeLocation("maskTexCoord"), BufferAttributeType::VEC2},
        { shader->GetAttributeLocation("maskTexID"), BufferAttributeType::FLOAT}
        });
    vao->AddVertexBuffer(vbo);
    vbo->Disable();

    indices = new uint[RENDERER_INDICES_SIZE];
    ibo = Buffer::Create(sizeof(indices), BufferType::INDEX, BufferDrawType::DYNAMIC);
    ibo->Disable();
    vao->SetIndexBuffer(ibo);
    vao->Disable();
    shader->Disable();
  }

  BatchRenderer::~BatchRenderer()
  {
    delete indices;
  }

  void BatchRenderer::Begin()
  {
    shader->Enable();
    vbo->Enable();
    m_buffer = (VertexData*)vbo->MapBuffer();
    m_iboSize = 0;
    m_lastIndex = 0;
  }

  void BatchRenderer::Draw(const Renderable2D& renderable)
  {

    const uint color = renderable.GetColor();

    const Vec2f texPos = renderable.GetTexPos();
    const Vec2f texSize = renderable.GetTexSize();

    const Vec2f maskTexPos = renderable.GetMaskTexPos();
    const Vec2f maskTexSize = renderable.GetMaskTexSize();

    const uint texID = renderable.GetTexID();
    const uint maskTexID = renderable.GetMaskTexID();
    uint ts = GetTextureSlot(texID);
    uint mts = GetTextureSlot(maskTexID);
    Draw(renderable.GetPosition(),renderable.GetSize(), texPos, texSize, ts, color, mts,maskTexPos,maskTexSize);
  }

  void BatchRenderer::DrawText(const std::string& text, const Vec2f& position, const Font& font, const uint& color)
  {
    float ts = GetTextureSlot(font.GetFontAtlasId());
    if (ts == 0 && font.GetFontAtlasId() != 0)
    {
      Flush();
      ts = GetTextureSlot(font.GetFontAtlasId());
    }

    const Ref<FontAtlas>& atlas = font.GetFontAtlas();
    const Vec2f& scale = Vec2f(1,1);//Vec2f(64.0, 64.0) / font.GetSize();//font.getScale();
    Vec2f pos;
    Vec2f size;
    Vec2f uv0;
    Vec2f uv1;
    Vec2f roundPos = Vec2f(round(position.x), round(position.y));
    float x = roundPos.x;
    for (uint i = 0;i < text.length();i++)
    {
      const Glyph& glyph = atlas->GetGlyph(text[i]);
      pos.x = x + glyph.miBearingX * scale.x;
      pos.y = roundPos.y - glyph.miBearingY * scale.y;
      size.x = glyph.miWidth * scale.x;
      size.y = glyph.miHeight * scale.y;

      uv0.x = glyph.mvTextureCoords.left;
      uv0.y = 1.0-glyph.mvTextureCoords.top;
      uv1.x = glyph.mvTextureCoords.right;
      uv1.y = 1.0-glyph.mvTextureCoords.bottom;

      AppendVertexBuffer(Vec2f(pos.x       ,pos.y       ), Vec2f(uv0.x,uv0.y),ts,color, 0, Vec2f(0, 0));
      AppendVertexBuffer(Vec2f(pos.x       ,pos.y+size.y), Vec2f(uv0.x,uv1.y),ts,color, 0, Vec2f(0, 0));
      AppendVertexBuffer(Vec2f(pos.x+size.x,pos.y+size.y), Vec2f(uv1.x,uv1.y),ts,color, 0, Vec2f(0, 0));
      AppendVertexBuffer(Vec2f(pos.x+size.x,pos.y       ), Vec2f(uv1.x,uv0.y),ts,color, 0, Vec2f(0, 0));

      AddIndicesPoly(4);
      m_iboSize += 6;

      x += glyph.miAdvanceX* scale.x;
    }
  }

  void BatchRenderer::DrawRect(const Transform2D& transform, uint texID, Vec2f texPos, Vec2f texSize, uint color, uint maskTexId, const Vec2f& maskTexPos, const Vec2f& maskTexSize)
  {
    Draw(transform, texPos, texSize, GetTextureSlot(texID), color, GetTextureSlot(maskTexId),maskTexPos,maskTexSize);
  }

  void BatchRenderer::DrawRect(const Vec2f& position,const Vec2f& size, uint texID, Vec2f texPos, Vec2f texSize, uint color,uint maskTexId, const Vec2f& maskTexPos, const Vec2f& maskTexSize)
  {
    uint ts = GetTextureSlot(texID);
    uint mts = GetTextureSlot(maskTexId);
    AppendVertexBuffer(Vec2f(position.x, position.y), Vec2f(texPos.x, texPos.y), ts, color, mts, Vec2f(maskTexPos.x, maskTexPos.y));
    AppendVertexBuffer(Vec2f(position.x, position.y + size.y), Vec2f(texPos.x, texPos.y + texSize.y), ts, color, mts, Vec2f(maskTexPos.x, maskTexPos.y + maskTexSize.y));
    AppendVertexBuffer(Vec2f(position.x + size.x, position.y + size.y), Vec2f(texPos.x + texSize.x, texPos.y + texSize.y), ts, color, mts, Vec2f(maskTexPos.x + maskTexSize.x, maskTexPos.y + maskTexSize.y));
    AppendVertexBuffer(Vec2f(position.x + size.x, position.y), Vec2f(texPos.x + texSize.x, texPos.y), ts, color, mts, Vec2f(maskTexPos.x + maskTexSize.x, maskTexPos.y));
    AddIndicesPoly(4);
    m_iboSize += 6;
  }

  void BatchRenderer::DrawRect(const Vec2f& position,const Vec2f& size, uint texID, Vec2f texPos, Vec2f texSize, uint color)
  {
    uint ts = GetTextureSlot(texID);
    AppendVertexBuffer(Vec2f(position.x, position.y), Vec2f(texPos.x, texPos.y), ts, color, 0, Vec2f(0,0));
    AppendVertexBuffer(Vec2f(position.x, position.y + size.y), Vec2f(texPos.x, texPos.y + texSize.y), ts, color, 0,Vec2f(0,0));
    AppendVertexBuffer(Vec2f(position.x + size.x, position.y + size.y), Vec2f(texPos.x + texSize.x, texPos.y + texSize.y), ts, color, 0,Vec2f(0,0));
    AppendVertexBuffer(Vec2f(position.x + size.x, position.y), Vec2f(texPos.x + texSize.x, texPos.y), ts, color, 0,Vec2f(0,0));
    AddIndicesPoly(4);
    m_iboSize += 6;
  }

  void BatchRenderer::Draw(const Vec2f& position,const Vec2f& size, const Vec2f& texPos, const Vec2f& texSize, const uint textureSlot, const uint color, const uint& maskTexSlot, const Vec2f& maskTexPos, const Vec2f& maskTexSize)
  {
    AppendVertexBuffer(Vec2f(position.x, position.y),Vec2f(texPos.x, texPos.y),textureSlot,color, maskTexSlot, Vec2f(maskTexPos.x, maskTexPos.y));
    AppendVertexBuffer(Vec2f(position.x, position.y+size.y),Vec2f(texPos.x, texPos.y + texSize.y),textureSlot,color, maskTexSlot, Vec2f(maskTexPos.x, maskTexPos.y+maskTexSize.y));
    AppendVertexBuffer(Vec2f(position.x+size.x, position.y+size.y),Vec2f(texPos.x + texSize.x, texPos.y + texSize.y),textureSlot,color, maskTexSlot, Vec2f(maskTexPos.x + maskTexSize.x, maskTexPos.y + maskTexSize.y));
    AppendVertexBuffer(Vec2f(position.x+size.x, position.y),Vec2f(texPos.x + texSize.x, texPos.y),textureSlot,color, maskTexSlot, Vec2f(maskTexPos.x + maskTexSize.x, maskTexPos.y));
    AddIndicesPoly(4);
    m_iboSize += 6;
  }

  void BatchRenderer::Draw(const Transform2D& transform, const Vec2f& texPos, const Vec2f& texSize, const uint textureSlot, const uint& color, const uint& maskTexSlot, const Vec2f& maskTexPos, const Vec2f& maskTexSize)
  {
    PushMatrix(transform.GetMatrix());

    AppendVertexBuffer(Vec2f(0, 0),Vec2f(texPos.x, texPos.y),textureSlot,color, maskTexSlot,Vec2f(maskTexPos.x, maskTexPos.y));
    AppendVertexBuffer(Vec2f(0, 1),Vec2f(texPos.x, texPos.y + texSize.y),textureSlot,color, maskTexSlot, Vec2f(maskTexPos.x, maskTexPos.y+maskTexSize.y));
    AppendVertexBuffer(Vec2f(1, 1),Vec2f(texPos.x + texSize.x, texPos.y + texSize.y),textureSlot,color, maskTexSlot, Vec2f(maskTexPos.x + maskTexSize.x, maskTexPos.y + maskTexSize.y));
    AppendVertexBuffer(Vec2f(1, 0),Vec2f(texPos.x + texSize.x, texPos.y),textureSlot,color, maskTexSlot, Vec2f(maskTexPos.x + maskTexSize.x, maskTexPos.y));

    PopMatrix();
    AddIndicesPoly(4);
    m_iboSize += 6;
  }

  void BatchRenderer::Draw(const Vec2f& position, const Vec2f* vertices, const uint amount, const uint color)
  {
    for (uint i = 0; i < amount; i++)
    {
      AppendVertexBuffer(position+vertices[i],Vec2f(0,0),0,color, 0, Vec2f(0, 0));
    }
    AddIndicesPoly(amount);
    m_iboSize += (amount-2)*3;
  }

  void BatchRenderer::DrawRect(const Vec2f& position, const Vec2f& size, const uint& color)
  {
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    AppendVertexBuffer(position, Vec2f(0, 0), 0, color, 0, Vec2f(0, 0));
    AppendVertexBuffer(Vec2f(position.x,position.y+size.y), Vec2f(0, 1), 0, color,0,Vec2f(0,1));
    AppendVertexBuffer(Vec2f(position.x+size.x, position.y + size.y), Vec2f(1, 1), 0, color, 0, Vec2f(1, 1));
    AppendVertexBuffer(Vec2f(position.x + size.x, position.y), Vec2f(1, 0), 0, color, 0, Vec2f(1, 0));

    AddIndicesPoly(4);
    m_iboSize += 6;

    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
  }

  void BatchRenderer::DrawLine(const Vec2f& pos1, const Vec2f& pos2, float width, uint color)
  {
    Vec2f delta = pos2-pos1;
    float alpha = atan(delta.y / delta.x);
    Vec2f p = Vec2f{sin(alpha),cos(alpha)} * width * 0.5f;

    // This if is needed to avoid culling problems
    if(delta.x >= 0)
    {
      AppendVertexBuffer(pos1+Vec2f{p.x, -p.y}, Vec2f(0, 0), 0, color, 0, Vec2f(0, 0));
      AppendVertexBuffer(pos1+Vec2f{-p.x, p.y}, Vec2f(0, 1), 0, color, 0, Vec2f(0, 1));
      AppendVertexBuffer(pos2+Vec2f{-p.x, p.y}, Vec2f(1, 1), 0, color, 0, Vec2f(1, 1));
      AppendVertexBuffer(pos2+Vec2f{p.x, -p.y}, Vec2f(1, 0), 0, color, 0, Vec2f(1, 0));
    }
    else
    {
      AppendVertexBuffer(pos2+Vec2f{p.x, -p.y}, Vec2f(0, 0), 0, color, 0, Vec2f(0, 0));
      AppendVertexBuffer(pos2+Vec2f{-p.x, p.y}, Vec2f(0, 1), 0, color, 0, Vec2f(0, 1));
      AppendVertexBuffer(pos1+Vec2f{-p.x, p.y}, Vec2f(1, 1), 0, color, 0, Vec2f(1, 1));
      AppendVertexBuffer(pos1+Vec2f{p.x, -p.y}, Vec2f(1, 0), 0, color, 0, Vec2f(1, 0));
    }

    AddIndicesPoly(4);
    m_iboSize += 6;
  }

  void BatchRenderer::FillRect(const Vec2f& position, const Vec2f& size, const uint& color)
  {
    AppendVertexBuffer(position, Vec2f(0, 0), 0, color, 0, Vec2f(0, 0));
    AppendVertexBuffer(Vec2f(position.x, position.y + size.y), Vec2f(0, 1), 0, color, 0, Vec2f(0, 1));
    AppendVertexBuffer(Vec2f(position.x + size.x, position.y + size.y), Vec2f(1, 1), 0, color, 0, Vec2f(1, 1));
    AppendVertexBuffer(Vec2f(position.x + size.x, position.y), Vec2f(1, 0), 0, color, 0, Vec2f(1, 0));

    AddIndicesPoly(4);
    m_iboSize += 6;
  }

  void BatchRenderer::FillRect(const Vec2f& position, const Vec2f& size, const uint& color, const Sprite* mask)
  {
    const Vec2f& m_maskTexPos = mask->GetTexPos();
    const Vec2f& m_maskTexSize = mask->GetTexSize();

    const uint& mtid = GetTextureSlot(mask->GetTextureID());

    AppendVertexBuffer(position, Vec2f(0, 0), 0, color, mtid, m_maskTexPos);
    AppendVertexBuffer(Vec2f(position.x, position.y + size.y), Vec2f(0, 1), 0, color, mtid, Vec2f(m_maskTexPos.x, m_maskTexPos.y+m_maskTexSize.y));
    AppendVertexBuffer(Vec2f(position.x + size.x, position.y + size.y), Vec2f(1, 1), 0, color, mtid, m_maskTexPos+m_maskTexSize);
    AppendVertexBuffer(Vec2f(position.x + size.x, position.y), Vec2f(1, 0), 0, color, mtid, Vec2f(m_maskTexPos.x + m_maskTexSize.x, m_maskTexPos.y));

    AddIndicesPoly(4);
    m_iboSize += 6;
  }

  void BatchRenderer::AppendVertexBuffer(const Vec2f& position, const Vec2f& texCoord, const uint& texID, const uint& color, const uint& maskTexId, const Vec2f& maskTexCoord)
  {
    m_buffer->vertex = *m_transformationBack*position;
    m_buffer->texCoord = texCoord;
    m_buffer->texID = texID;
    m_buffer->color = color;
    m_buffer->maskTexCoord = maskTexCoord;
    m_buffer->maskTexID = maskTexId;
    m_buffer++;
  }

  uint BatchRenderer::GetTextureSlot(const uint texID)
  {
    if (texID == 0)
      return 0.0f;
    uint ts = 0.0f;
    bool found = false;
    const int size = m_texSlots.size();
    for (int i = 0; i < size; i++)
    {
      if (m_texSlots[i] == texID)
      {
        ts = (uint)(i + 1);
        found = true;
        break;
      }
    }
    if (!found)
    {
      if (size >= maxTextures)
      {
        End();
        Flush();
        Begin();
      }
      m_texSlots.push_back(texID);
      ts = (uint)(size + 1);
    }
    return ts;
  }

  void BatchRenderer::End()
  {
    vbo->UnmapBuffer();
    vbo->Disable();
  }

  void BatchRenderer::Flush()
  {
    RenderCommand::EnableDepthTest(false);
    for (uint i = 0; i < m_texSlots.size(); i++)
    {
      GLCall(glActiveTexture(GL_TEXTURE0 + i));
      GLCall(glBindTexture(GL_TEXTURE_2D, m_texSlots[i]));
    }

    EnableBuffers();
    vao->Render(DrawType::TRIANGLES, m_iboSize);
    DisableBuffers();

    m_iboSize = 0;
    m_texSlots.clear();
    GLCall(glActiveTexture(GL_TEXTURE0));
    RenderCommand::ResetDepthTest();
    shader->Disable();
  }

  void BatchRenderer::EnableBuffers()
  {
    vao->Enable();
    ibo->UpdateData(indices, m_iboSize*sizeof(uint));
  }

  void BatchRenderer::DisableBuffers()
  {
    vao->Disable();
  }

  void BatchRenderer::AddIndicesPoly(uint vertices)
  {
    vertices -= 2;
    for (uint i = 0; i < vertices; i++)
    {
      indices[m_iboSize + 3 * i] = m_lastIndex;
      indices[m_iboSize + 3 * i + 1] = m_lastIndex + i+1;
      indices[m_iboSize + 3 * i + 2] = m_lastIndex + i+2;

    }
    m_lastIndex = indices[m_iboSize + 3 * (vertices - 1) + 2] + 1;
  }
}
