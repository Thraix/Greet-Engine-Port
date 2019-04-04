#pragma once

#include <math/Maths.h>
#include <internal/GreetGL.h>
#include <internal/GreetTypes.h>
#include <vector>
#include <functional>

namespace Greet {

  struct AttributeDefaults
  {
    const uint location;
    const uint vertexValueSize;
    const uint memoryValueSize;
    const uint glType;
    const bool normalized;
    AttributeDefaults(uint location, uint vertexValueSize, uint memoryValueSize, uint glType, bool normalized)
      : location(location), vertexValueSize(vertexValueSize), memoryValueSize(memoryValueSize), glType(glType), normalized(normalized)
    {
    }

  };

  struct AttributeDataBase
  {
    public:
      const uint location;
      const uint vertexValueSize;
      const uint memoryValueSize;
      const uint glType;
      const bool normalized;
      void* data;
    protected:
      AttributeDataBase(AttributeDefaults defaults, void* data) : location(defaults.location), vertexValueSize(defaults.vertexValueSize), memoryValueSize(defaults.memoryValueSize), glType(defaults.glType), normalized(defaults.normalized), data(data){}


    public:
      virtual ~AttributeDataBase() {}
  };


  // Implementation of AttributeDataBase (mainly destructor)
  template <typename T>
  struct AttributeData : public AttributeDataBase
  {
    AttributeData(AttributeDefaults defaults, T* data) 
      : AttributeDataBase(defaults,data) 
    {}

    virtual ~AttributeData()
    {
      delete[] (T*)data;
    }
  };



  class MeshData
  {
    friend class Mesh;
    private:
      std::vector<AttributeDataBase*> m_data;
      Vec3<float>* m_vertices;
      uint* m_indices;
      uint m_vertexCount;
      uint m_indexCount;
    public:
      MeshData(Vec3<float>* vertices, uint vertexCount, uint* indices, uint indexCount);
      virtual ~MeshData();
      void AddAttribute(AttributeDataBase* data);
      AttributeDataBase* GetAttribute(AttributeDefaults defaults) const;
      AttributeDataBase* RemoveAttribute(AttributeDefaults defaults);

      Vec3<float>* GetVertices() const { return m_vertices; }
      uint* GetIndices() const { return m_indices; }
      uint GetVertexCount() const { return m_vertexCount; }
      uint GetIndexCount() const { return m_indexCount; }

      MeshData* LowPolify();

      void WriteToFile(const std::string& filename) const;
      static MeshData* ReadFromFile(const std::string& filename);
  };
}

#define ATTRIBUTE_COLOR		Greet::AttributeDefaults(MESH_COLORS_LOCATION, 4, sizeof(unsigned int), GL_UNSIGNED_BYTE,GL_TRUE)
#define ATTRIBUTE_NORMAL	Greet::AttributeDefaults(MESH_NORMALS_LOCATION,3, sizeof(Greet::Vec3<float>) ,GL_FLOAT,GL_FALSE)
#define ATTRIBUTE_TEXCOORD	Greet::AttributeDefaults(MESH_TEXCOORDS_LOCATION,2, sizeof(Greet::Vec2),GL_FLOAT,GL_FALSE)
