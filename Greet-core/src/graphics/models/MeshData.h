#pragma once

#include <math/Maths.h>
#include <internal/GreetGL.h>
#include <vector>
#include <functional>

#define MESH_VERTICES_LOCATION	0
#define MESH_TEXCOORDS_LOCATION 1
#define MESH_COLORS_LOCATION	2 
#define MESH_NORMALS_LOCATION	3

#define ATTRIBUTE_COLOR		Greet::AttributeDefaults(MESH_COLORS_LOCATION, 4, sizeof(unsigned int), GL_UNSIGNED_BYTE,GL_TRUE)
#define ATTRIBUTE_NORMAL	Greet::AttributeDefaults(MESH_NORMALS_LOCATION,3, sizeof(Greet::Vec3<float>) ,GL_FLOAT,GL_FALSE)
#define ATTRIBUTE_TEXCOORD	Greet::AttributeDefaults(MESH_TEXCOORDS_LOCATION,2, sizeof(Greet::Vec2),GL_FLOAT,GL_FALSE)

namespace Greet {

  struct AttributeDefaults
  {
    uint location;
    uint vertexValueSize;
    uint memoryValueSize;
    uint glType;
    bool normalized;
    AttributeDefaults(uint location, uint vertexValueSize, uint memoryValueSize, uint glType, bool normalized)
      : location(location), vertexValueSize(vertexValueSize), memoryValueSize(memoryValueSize), glType(glType), normalized(normalized)
    {
    }

  };

  struct AttributeData
  {
    public:
      uint location;
      uint vertexValueSize;
      uint memoryValueSize;
      uint glType;
      bool normalized;
      std::vector<char> data;
    public:
      template <typename T>
      AttributeData(AttributeDefaults defaults, const std::vector<T>& data) 
      : location(defaults.location), vertexValueSize(defaults.vertexValueSize), 
      memoryValueSize(defaults.memoryValueSize), glType(defaults.glType), 
      normalized(defaults.normalized), data{(char*)data.data(), (char*)data.data() + data.size() * sizeof(T)}
    {
    }

    public:
      virtual ~AttributeData() {}
  };

  class MeshData
  {
    friend class Mesh;
    private:
      std::vector<Vec3<float>> m_vertices;
      std::vector<uint> m_indices;
      std::vector<AttributeData> m_data;
    public:
      MeshData(const std::vector<Vec3<float>>& vertices, const std::vector<uint>& indices);
      MeshData(std::vector<Vec3<float>>&& vertices, std::vector<uint>&& indices);
      virtual ~MeshData();
      void AddAttribute(AttributeData&& data);
      const AttributeData* GetAttribute(AttributeDefaults defaults) const;
      AttributeData* RemoveAttribute(AttributeDefaults defaults);

      const std::vector<Vec3<float>>& GetVertices() const { return m_vertices; }
      const std::vector<uint>& GetIndices() const { return m_indices; }
      std::vector<Vec3<float>>& GetVertices() { return m_vertices; }
      std::vector<uint>& GetIndices() { return m_indices; }
      uint GetVertexCount() const { return m_vertices.size(); }
      uint GetIndexCount() const { return m_indices.size(); }

      void GenerateNormals();
      MeshData* LowPolify();

      void WriteToFile(const std::string& filename) const;
      static MeshData ReadFromFile(const std::string& filename);
  };
}
