#include "Mesh.h"

namespace Greet {

	Mesh::Mesh(const Vec3* vertices, uint vertexCount, const uint* indices, uint indexCount)
	{
		init(vertices, vertexCount, indices, indexCount);
	}

	Mesh::Mesh(MeshData* data)
	{
		init(data->GetVertices(),data->GetVertexCount(),data->GetIndices(),data->GetIndexCount());
		for (uint i = 0;i < data->m_data.size();i++)
		{
			AddAttribute(data->m_data[i]);
		}
	}

	void Mesh::init(const Vec3* vertices, uint vertexCount, const uint* indices, uint indexCount)
	{
		m_drawMode = GL_TRIANGLES;
		m_vertexCount = vertexCount;
		m_indexCount = indexCount;

		// VAO
		GLCall(glGenVertexArrays(1, &m_vaoId));
		GLCall(glBindVertexArray(m_vaoId));

		// IBO
		GLCall(glGenBuffers(1, &m_iboId));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint), indices, GL_STATIC_DRAW));

		// Attributes 
		AddAttribute(MESH_VERTICES_LOCATION, vertices); // vertices
		
		// Set default color to white
		GLCall(glVertexAttrib4f(MESH_COLORS_LOCATION,1.0f,1.0f,1.0f,1.0f));

		// Unbind
		GLCall(glBindVertexArray(0));
	}
	Mesh::~Mesh()
	{
		for (auto it = m_vbos.begin();it != m_vbos.end(); it++)
		{
			GLCall(glDeleteBuffers(1,&it->second));
		}
		m_vbos.clear();
		GLCall(glDeleteBuffers(1,&m_iboId));
		GLCall(glDeleteVertexArrays(1,&m_vaoId));
	}

	void Mesh::Render() const
	{
		GLCall(glDrawElements(m_drawMode, m_indexCount * sizeof(uint), GL_UNSIGNED_INT,0));
	}

	void Mesh::Bind() const
	{
		if (m_culling)
		{	
			GLCall(glEnable(GL_CULL_FACE));
			GLCall(glFrontFace(m_clockwise ? GL_CW : GL_CCW));
		}
		else
		{
			GLCall(glDisable(GL_CULL_FACE));
		}

		GLCall(glBindVertexArray(m_vaoId));
		EnableAttributes();
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId));
	}

	void Mesh::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		DisableAttributes();
		GLCall(glBindVertexArray(0));
		GLCall(glDisable(GL_CULL_FACE));
	}

	void Mesh::EnableAttributes() const
	{
		for (auto it = m_vbos.begin();it != m_vbos.end(); it++)
		{
			GLCall(glEnableVertexAttribArray(it->first));
		}	
	}

	void Mesh::DisableAttributes() const
	{
		for (auto it = m_vbos.begin();it != m_vbos.end(); it++)
		{
			GLCall(glDisableVertexAttribArray(it->first));
		}
	}

	void Mesh::AddAttribute(uint location, const Vec3* data)
	{

		if (m_vbos.find(location) != m_vbos.end())
		{
			ErrorHandle::SetErrorCode(GREET_ERROR_MESH_LOCATION);
			Log::Error("Shader location already used in mesh: ", location);
			return;
		}
		GLCall(glBindVertexArray(m_vaoId));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId));
		uint vbo;
		GLCall(glGenBuffers(1, &vbo));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		m_vbos.emplace(location, vbo); // Needed to delete vbo when deleting mesh
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(float) * 3, data, GL_STATIC_DRAW));
		GLCall(glVertexAttribPointer(location, 3, GL_FLOAT, false, 0, 0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		GLCall(glBindVertexArray(0));
	}

	void Mesh::AddAttribute(uint location, const Vec2* data)
	{
		if (m_vbos.find(location) != m_vbos.end())
		{
			ErrorHandle::SetErrorCode(GREET_ERROR_MESH_LOCATION);
			Log::Error("Shader location already used in mesh: ", location);
			return;
		}
		GLCall(glBindVertexArray(m_vaoId));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId));
		uint vbo;
		GLCall(glGenBuffers(1, &vbo));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		m_vbos.emplace(location, vbo); // Needed to delete vbo when deleting mesh
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(float) * 2, data, GL_STATIC_DRAW));
		GLCall(glVertexAttribPointer(location, 2, GL_FLOAT, false, 0, 0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		GLCall(glBindVertexArray(0));
	}

	void Mesh::AddAttribute(uint location, uint attributeSize, const uint* data)
	{
		if (m_vbos.find(location) != m_vbos.end())
		{
			ErrorHandle::SetErrorCode(GREET_ERROR_MESH_LOCATION);
			Log::Error("Shader location already used in mesh: ",location);
			return;
		}
		GLCall(glBindVertexArray(m_vaoId));
		uint vbo;
		GLCall(glGenBuffers(1, &vbo));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		m_vbos.emplace(location, vbo); // Needed to delete vbo when deleting mesh
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(byte) * attributeSize, data, GL_STATIC_DRAW));
		GLCall(glVertexAttribPointer(location, attributeSize, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		GLCall(glBindVertexArray(0));

	}

	void Mesh::AddAttribute(AttributeData* data)
	{
		if (m_vbos.find(data->location) != m_vbos.end())
		{
			ErrorHandle::SetErrorCode(GREET_ERROR_MESH_LOCATION);
			Log::Error("Shader location already used in mesh: ",data->location);
			return;
		}
		GLCall(glBindVertexArray(m_vaoId));
		uint vbo;
		GLCall(glGenBuffers(1, &vbo));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		m_vbos.emplace(data->location,vbo); // Needed to delete vbo when deleting mesh
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_vertexCount * data->memoryValueSize, data->data, GL_STATIC_DRAW));
		GLCall(glVertexAttribPointer(data->location, data->vertexValueSize, data->glType, data->normalized, 0, 0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER,0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0));
		GLCall(glBindVertexArray(0));
	}

	void Mesh::SetDefaultAttribute4f(uint location, const Vec4& data)
	{
		GLCall(glBindVertexArray(m_vaoId));
		GLCall(glVertexAttrib4f(location,data.x,data.y,data.z,data.w));
		GLCall(glBindVertexArray(0));
	}

	void Mesh::SetDefaultAttribute3f(uint location, const Vec3& data)
	{
		GLCall(glBindVertexArray(m_vaoId));
		GLCall(glVertexAttrib3f(location,data.x,data.y,data.z));
		GLCall(glBindVertexArray(0));
	}
}