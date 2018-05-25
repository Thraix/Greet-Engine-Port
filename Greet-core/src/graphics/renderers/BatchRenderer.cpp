#include "BatchRenderer.h"

namespace Greet {

	BatchRenderer::BatchRenderer()
	{
		Init();
	}

	BatchRenderer::~BatchRenderer()
	{
		delete m_indices;
		GLCall(glDeleteBuffers(1, &m_ibo));
		GLCall(glDeleteBuffers(1, &m_vbo));
		GLCall(glDeleteVertexArrays(1, &m_vao));
		for (uint i = 0; i < m_texSlots.size(); i++)
		{
			delete &m_texSlots[i];
		}
	}

	void BatchRenderer::Init()
	{
		GLCall(glGenVertexArrays(1, &m_vao));
		GLCall(glGenBuffers(1, &m_vbo));

		GLCall(glBindVertexArray(m_vao));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));

		GLCall(glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW));

		GLCall(glEnableVertexAttribArray(SHADER_VERTEX_INDEX));
		GLCall(glEnableVertexAttribArray(SHADER_TEXCOORD_INDEX));
		GLCall(glEnableVertexAttribArray(SHADER_TEXID_INDEX));
		GLCall(glEnableVertexAttribArray(SHADER_COLOR_INDEX));
		GLCall(glEnableVertexAttribArray(SHADER_MASK_TEXCOORD_INDEX));
		GLCall(glEnableVertexAttribArray(SHADER_MASK_TEXID_INDEX));
		GLCall(glVertexAttribPointer(SHADER_VERTEX_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)offsetof(VertexData, VertexData::vertex)));
		GLCall(glVertexAttribPointer(SHADER_TEXCOORD_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)offsetof(VertexData, VertexData::texCoord)));
		GLCall(glVertexAttribPointer(SHADER_TEXID_INDEX, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)offsetof(VertexData, VertexData::texID)));
		GLCall(glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (const GLvoid*)offsetof(VertexData, VertexData::color)));
		GLCall(glVertexAttribPointer(SHADER_MASK_TEXCOORD_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)offsetof(VertexData, VertexData::maskTexCoord)));
		GLCall(glVertexAttribPointer(SHADER_MASK_TEXID_INDEX, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)offsetof(VertexData, VertexData::maskTexID)));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

		//Generate all the indices at runtime
		m_indices = new GLuint[RENDERER_INDICES_SIZE];
		GLCall(glGenBuffers(1, &m_ibo));
		GLCall(glBindVertexArray(0));
	}

	void BatchRenderer::Begin()
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
		GLCall(m_buffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		m_iboSize = 0;
		m_lastIndex = 0;
	}

	void BatchRenderer::Submit(const RenderablePoly* renderable)
	{
		const uint color = renderable->GetColor();
		const Vec2* vertices = renderable->GetVertices();
		const uint vertexCount = renderable->GetVertexCount();
		Draw(renderable->GetPosition(),vertices, vertexCount, color);
	}

	void BatchRenderer::Submit(const Renderable2D* renderable)
	{

		const uint color = renderable->GetColor();

		const Vec2 texPos = renderable->GetTexPos();
		const Vec2 texSize = renderable->GetTexSize();

		const Vec2 maskTexPos = renderable->GetMaskTexPos();
		const Vec2 maskTexSize = renderable->GetMaskTexSize();

		const GLuint texID = renderable->GetTexID();
		const GLuint maskTexID = renderable->GetMaskTexID();
		uint ts = GetTextureSlot(texID);
		uint mts = GetTextureSlot(maskTexID);
		Draw(renderable->GetPosition(),renderable->GetSize(), texPos, texSize, ts, color, mts,maskTexPos,maskTexSize);
	}

	void BatchRenderer::SubmitString(const std::string& text, const Vec2& position, Font* font, const uint& color)
	{
		using namespace ftgl;
		uint ts = GetTextureSlot(font->GetAtlasID());
		texture_font_t* ftfont = font->GetFTFont();
		float x = position.x;
		const Vec2& scale = Vec2(1,1);//font->getScale();
		float baseline = ftfont->size - ftfont->ascender;
    const char* str = text.c_str();
		for(uint i = 0;i<text.length();i++)
		{
			const char& c = text[i];
			texture_glyph_t* glyph = texture_font_get_glyph(ftfont,str+i);
			if(glyph != NULL)
			{
				if (i > 0)
				{
					float kerning = texture_glyph_get_kerning(glyph,str+i-1);
					x += kerning / scale.x;
				}
				float x0 = x + glyph->offset_x / scale.x;
				float y0 = position.y - glyph->offset_y / scale.y;
				float x1 = x0 + glyph->width / scale.x;
				float y1 = y0 + glyph->height / scale.y;
					
					
				float u0 = glyph->s0;
				float v0 = 1-glyph->t0;
				float u1 = glyph->s1;
				float v1 = 1-glyph->t1;
				AppendVertexBuffer(Vec2(x0,y0),Vec2(u0,v0),ts,color, 0, Vec2(0, 0));
				AppendVertexBuffer(Vec2(x0,y1),Vec2(u0,v1),ts,color, 0, Vec2(0, 0));
				AppendVertexBuffer(Vec2(x1,y1),Vec2(u1,v1),ts,color, 0, Vec2(0, 0));
				AppendVertexBuffer(Vec2(x1,y0),Vec2(u1,v0),ts,color, 0, Vec2(0, 0));
				x += glyph->advance_x / scale.x;
				AddIndicesPoly(4);
				m_iboSize += 6;
			}
		}
	}

	void BatchRenderer::Submit(const Transform& transform, uint texID, Vec2 texPos, Vec2 texSize, uint color, uint maskTexId, const Vec2& maskTexPos, const Vec2& maskTexSize)
	{
		Draw(transform, texPos, texSize, GetTextureSlot(texID), color, GetTextureSlot(maskTexId),maskTexPos,maskTexSize);
	}

	void BatchRenderer::Submit(const Vec2& position,const Vec2& size, uint texID, Vec2 texPos, Vec2 texSize, uint color,uint maskTexId, const Vec2& maskTexPos, const Vec2& maskTexSize)
	{
		uint ts = GetTextureSlot(texID);
		uint mts = GetTextureSlot(maskTexId);
		AppendVertexBuffer(Vec2(position.x, position.y), Vec2(texPos.x, texPos.y), ts, color, mts, Vec2(maskTexPos.x, maskTexPos.y));
		AppendVertexBuffer(Vec2(position.x, position.y + size.y), Vec2(texPos.x, texPos.y + texSize.y), ts, color, mts, Vec2(maskTexPos.x, maskTexPos.y + maskTexSize.y));
		AppendVertexBuffer(Vec2(position.x + size.x, position.y + size.y), Vec2(texPos.x + texSize.x, texPos.y + texSize.y), ts, color, mts, Vec2(maskTexPos.x + maskTexSize.x, maskTexPos.y + maskTexSize.y));
		AppendVertexBuffer(Vec2(position.x + size.x, position.y), Vec2(texPos.x + texSize.x, texPos.y), ts, color, mts, Vec2(maskTexPos.x + maskTexSize.x, maskTexPos.y));
		AddIndicesPoly(4);
		m_iboSize += 6;
	}

	void BatchRenderer::Draw(const Vec2& position,const Vec2& size, const Vec2& texPos, const Vec2& texSize, const uint textureSlot, const uint color, const uint& maskTexSlot, const Vec2& maskTexPos, const Vec2& maskTexSize)
	{
		AppendVertexBuffer(Vec2(position.x, position.y),Vec2(texPos.x, texPos.y),textureSlot,color, maskTexSlot, Vec2(maskTexPos.x, maskTexPos.y));
		AppendVertexBuffer(Vec2(position.x, position.y+size.y),Vec2(texPos.x, texPos.y + texSize.y),textureSlot,color, maskTexSlot, Vec2(maskTexPos.x, maskTexPos.y+maskTexSize.y));
		AppendVertexBuffer(Vec2(position.x+size.x, position.y+size.y),Vec2(texPos.x + texSize.x, texPos.y + texSize.y),textureSlot,color, maskTexSlot, Vec2(maskTexPos.x + maskTexSize.x, maskTexPos.y + maskTexSize.y));
		AppendVertexBuffer(Vec2(position.x+size.x, position.y),Vec2(texPos.x + texSize.x, texPos.y),textureSlot,color, maskTexSlot, Vec2(maskTexPos.x + maskTexSize.x, maskTexPos.y));
		AddIndicesPoly(4);
		m_iboSize += 6;
	}

	void BatchRenderer::Draw(const Transform& transform, const Vec2& texPos, const Vec2& texSize, const uint textureSlot, const uint& color, const uint& maskTexSlot, const Vec2& maskTexPos, const Vec2& maskTexSize)
	{
		PushMatrix(transform.GetMatrix());
			
		AppendVertexBuffer(Vec2(0, 0),Vec2(texPos.x, texPos.y),textureSlot,color, maskTexSlot,Vec2(maskTexPos.x, maskTexPos.y));
		AppendVertexBuffer(Vec2(0, 1),Vec2(texPos.x, texPos.y + texSize.y),textureSlot,color, maskTexSlot, Vec2(maskTexPos.x, maskTexPos.y+maskTexSize.y));
		AppendVertexBuffer(Vec2(1, 1),Vec2(texPos.x + texSize.x, texPos.y + texSize.y),textureSlot,color, maskTexSlot, Vec2(maskTexPos.x + maskTexSize.x, maskTexPos.y + maskTexSize.y));
		AppendVertexBuffer(Vec2(1, 0),Vec2(texPos.x + texSize.x, texPos.y),textureSlot,color, maskTexSlot, Vec2(maskTexPos.x + maskTexSize.x, maskTexPos.y));
			
		PopMatrix();
		AddIndicesPoly(4);
		m_iboSize += 6;
	}

	void BatchRenderer::Draw(const Vec2& position, const Vec2* vertices, const uint amount, const uint color)
	{
		for (uint i = 0; i < amount; i++)
		{
			AppendVertexBuffer(position+vertices[i],Vec2(0,0),0,color, 0, Vec2(0, 0));
		}
		AddIndicesPoly(amount);
		m_iboSize += (amount-2)*3;
	}

	void BatchRenderer::DrawRect(const Vec2& position, const Vec2& size, const uint& color)
	{
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		AppendVertexBuffer(position, Vec2(0, 0), 0, color, 0, Vec2(0, 0));
		AppendVertexBuffer(Vec2(position.x,position.y+size.y), Vec2(0, 1), 0, color,0,Vec2(0,1));
		AppendVertexBuffer(Vec2(position.x+size.x, position.y + size.y), Vec2(1, 1), 0, color, 0, Vec2(1, 1));
		AppendVertexBuffer(Vec2(position.x + size.x, position.y), Vec2(1, 0), 0, color, 0, Vec2(1, 0));

		AddIndicesPoly(4);
		m_iboSize += 6;
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	}

	void BatchRenderer::FillRect(const Vec2& position, const Vec2& size, const uint& color)
	{
		AppendVertexBuffer(position, Vec2(0, 0), 0, color, 0, Vec2(0, 0));
		AppendVertexBuffer(Vec2(position.x, position.y + size.y), Vec2(0, 1), 0, color, 0, Vec2(0, 1));
		AppendVertexBuffer(Vec2(position.x + size.x, position.y + size.y), Vec2(1, 1), 0, color, 0, Vec2(1, 1));
		AppendVertexBuffer(Vec2(position.x + size.x, position.y), Vec2(1, 0), 0, color, 0, Vec2(1, 0));

		AddIndicesPoly(4);
		m_iboSize += 6;
	}

	void BatchRenderer::FillRect(const Vec2& position, const Vec2& size, const uint& color, const Sprite* mask)
	{
		const Vec2& m_maskTexPos = mask->GetTexPos();
		const Vec2& m_maskTexSize = mask->GetTexSize();

		const uint& mtid = GetTextureSlot(mask->GetTextureID());

		AppendVertexBuffer(position, Vec2(0, 0), 0, color, mtid, m_maskTexPos);
		AppendVertexBuffer(Vec2(position.x, position.y + size.y), Vec2(0, 1), 0, color, mtid, Vec2(m_maskTexPos.x, m_maskTexPos.y+m_maskTexSize.y));
		AppendVertexBuffer(Vec2(position.x + size.x, position.y + size.y), Vec2(1, 1), 0, color, mtid, m_maskTexPos+m_maskTexSize);
		AppendVertexBuffer(Vec2(position.x + size.x, position.y), Vec2(1, 0), 0, color, mtid, Vec2(m_maskTexPos.x + m_maskTexSize.x, m_maskTexPos.y));

		AddIndicesPoly(4);
		m_iboSize += 6;
	}
		
	void BatchRenderer::AppendVertexBuffer(const Vec2& position, const Vec2& texCoord, const uint& texID, const uint& color, const uint& maskTexId, const Vec2& maskTexCoord)
	{
		m_buffer->vertex = *m_transformationBack*position;
		m_buffer->texCoord = texCoord;
		m_buffer->texID = texID;
		m_buffer->color = color;
		m_buffer->maskTexCoord = maskTexCoord;
		m_buffer->maskTexID = maskTexId;
		m_buffer++;
	}

	uint BatchRenderer::GetTextureSlot(const GLuint texID)
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
			if (size >= RENDERER_MAX_TEXTURES)
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
		GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void BatchRenderer::Flush()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
		for (uint i = 0; i < m_texSlots.size(); i++)
		{
			GLCall(glActiveTexture(GL_TEXTURE0 + i));
			GLCall(glBindTexture(GL_TEXTURE_2D, m_texSlots[i]));
		}

		EnableBuffers();

		GLCall(glDrawElements(GL_TRIANGLES, m_iboSize, GL_UNSIGNED_INT, NULL));

		DisableBuffers();

		m_iboSize = 0;
		m_texSlots.clear();
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glEnable(GL_DEPTH_TEST));
	}

	void BatchRenderer::EnableBuffers()
	{
		GLCall(glBindVertexArray(m_vao));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_iboSize*sizeof(uint), m_indices, GL_DYNAMIC_DRAW));
	}

	void BatchRenderer::DisableBuffers()
	{
		GLCall(glBindVertexArray(0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	void BatchRenderer::AddIndicesPoly(uint vertices)
	{
		vertices -= 2;
		for (uint i = 0; i < vertices; i++)
		{
			m_indices[m_iboSize + 3 * i] = m_lastIndex;
			m_indices[m_iboSize + 3 * i + 1] = m_lastIndex + i+1;
			m_indices[m_iboSize + 3 * i + 2] = m_lastIndex + i+2;

		}
		m_lastIndex = m_indices[m_iboSize + 3 * (vertices - 1) + 2] + 1;
	}
}
