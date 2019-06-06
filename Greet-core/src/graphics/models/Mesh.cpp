#include "Mesh.h"

#include <utils/ErrorHandle.h>
#include <internal/GreetGL.h>
#include <logging/Logger.h>

namespace Greet {

  Mesh::Mesh(const std::vector<Vec3<float>>& vertices, const std::vector<uint>& indices)
    : vao{}, ibo{(uint)(indices.size()*sizeof(uint)), BufferType::INDEX, BufferDrawType::STATIC}
  {
    m_drawMode = GL_TRIANGLES;
    m_vertexCount = vertices.size();
    m_indexCount = indices.size();

    vao.Enable();
    ibo.Enable();
    ibo.UpdateData((void*)indices.data());

    // Attributes 
    AddAttribute(MESH_VERTICES_LOCATION, vertices.data()); // vertices

    // Set default color to white
    GLCall(glVertexAttrib4f(MESH_COLORS_LOCATION,1.0f,1.0f,1.0f,1.0f));

    // Unbind
    vao.Disable();
    ibo.Disable();
  }

  Mesh::Mesh(MeshData* data)
    : Mesh(data->GetVertices(), data->GetIndices())
  {
    for (uint i = 0;i < data->m_data.size();i++)
    {
      AddAttribute(data->m_data[i]);
    }
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

    if(wireframe) 
    {
      GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    }

    vao.Enable();
    EnableAttributes();
    ibo.Enable();
  }

  void Mesh::Unbind() const
  {
    ibo.Disable();
    DisableAttributes();
    vao.Disable();
    GLCall(glEnable(GL_CULL_FACE));
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    GLCall(glFrontFace(GL_CCW));
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

  void Mesh::AddAttribute(uint location, const Vec3<float>* data)
  {
    AddAttribute(location, (void*)data, sizeof(Vec3<float>), 3, GL_FLOAT, false);
  }

  void Mesh::AddAttribute(uint location, const Vec2* data)
  {
    AddAttribute(location, (void*)data, sizeof(Vec2), 2, GL_FLOAT, false);
  }

  void Mesh::AddAttribute(uint location, uint attributeSize, const uint* data)
  {
    AddAttribute(location, (void*)data, sizeof(byte), attributeSize, GL_UNSIGNED_BYTE, true);
  }

  void Mesh::AddAttribute(AttributeData* attr)
  {
    AddAttribute(attr->location, attr->data.data(), attr->memoryValueSize, attr->vertexValueSize, attr->glType, attr->normalized);
  }

  void Mesh::AddAttribute(uint location, void* data, uint typeSize, uint typeCount, uint glType, bool normalized)
  {
    if (HasVBO(location))
      return;

    vao.Enable();
    ibo.Enable();

    Buffer b{m_vertexCount * typeSize, BufferType::ARRAY, BufferDrawType::STATIC};
    b.Enable();
    b.UpdateData(data);

    GLCall(glVertexAttribPointer(location, typeCount, glType, normalized, 0, 0));
    b.Disable();
    m_vbos.emplace(location, std::move(b));
    ibo.Disable();
    vao.Disable();
  }

  bool Mesh::HasVBO(uint location) const
  {
    if (m_vbos.find(location) != m_vbos.end())
    {
      ErrorHandle::SetErrorCode(GREET_ERROR_MESH_LOCATION);
      Log::Error("Shader location already used in mesh: ",location);
      return true;
    }
    return false;
  }

  void Mesh::SetDefaultAttribute4f(uint location, const Vec4& data)
  {
    vao.Enable();
    GLCall(glVertexAttrib4f(location,data.x,data.y,data.z,data.w));
    vao.Disable();
  }

  void Mesh::SetDefaultAttribute3f(uint location, const Vec3<float>& data)
  {
    vao.Enable();
    GLCall(glVertexAttrib3f(location,data.x,data.y,data.z));
    vao.Disable();
  }
}
