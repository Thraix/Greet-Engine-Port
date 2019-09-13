#pragma once

#include <common/Types.h>
#include <common/Memory.h>
#include <graphics/buffers/Buffer.h>

namespace Greet
{
  enum class BufferAttributeType
  {
    FLOAT, VEC2, VEC3, VEC4, MAT3, MAT4, UINT, UINT2, UINT3, UINT4
  };

  static uint BufferAttributeToCount(BufferAttributeType type)
  {
    switch(type)
    {
      case BufferAttributeType::FLOAT: return 1;
      case BufferAttributeType::VEC2:  return 2;
      case BufferAttributeType::VEC3:  return 3;
      case BufferAttributeType::VEC4:  return 4;
      case BufferAttributeType::MAT3:  return 9;
      case BufferAttributeType::MAT4:  return 16;
      case BufferAttributeType::UINT:  return 1;
      case BufferAttributeType::UINT2: return 2;
      case BufferAttributeType::UINT3: return 3;
      case BufferAttributeType::UINT4: return 4;
    }

    ASSERT(false, "Unsupported BufferAttributeType");
    return 0;
  }

  static uint BufferAttributeToByteSize(BufferAttributeType type)
  {
    switch(type)
    {
      case BufferAttributeType::FLOAT: return 1 * sizeof(float);
      case BufferAttributeType::VEC2:  return 2 * sizeof(float);
      case BufferAttributeType::VEC3:  return 3 * sizeof(float);
      case BufferAttributeType::VEC4:  return 4 * sizeof(float);
      case BufferAttributeType::MAT3:  return 9 * sizeof(float);
      case BufferAttributeType::MAT4:  return 16 * sizeof(float);
      case BufferAttributeType::UINT:  return 1 * sizeof(int);
      case BufferAttributeType::UINT2: return 2 * sizeof(int);
      case BufferAttributeType::UINT3: return 3 * sizeof(int);
      case BufferAttributeType::UINT4: return 4 * sizeof(int);
    }

    ASSERT(false, "Unsupported BufferAttributeType");
    return 0;
  }

  static uint BufferAttributeToGLPrimitive(BufferAttributeType type)
  {
    switch(type)
    {
      case BufferAttributeType::FLOAT: return GL_FLOAT;
      case BufferAttributeType::VEC2:  return GL_FLOAT;
      case BufferAttributeType::VEC3:  return GL_FLOAT;
      case BufferAttributeType::VEC4:  return GL_FLOAT;
      case BufferAttributeType::MAT3:  return GL_FLOAT;
      case BufferAttributeType::MAT4:  return GL_FLOAT;
      case BufferAttributeType::UINT:  return GL_UNSIGNED_INT;
      case BufferAttributeType::UINT2: return GL_UNSIGNED_INT;
      case BufferAttributeType::UINT3: return GL_UNSIGNED_INT;
      case BufferAttributeType::UINT4: return GL_UNSIGNED_INT;
    }

    ASSERT(false, "Unsupported BufferAttributeType");
    return 0;
  }

  struct BufferAttribute
  {
    uint location;
    BufferAttributeType type;
    uint size;
    uint offset;
    bool normalize;

    BufferAttribute(uint location, BufferAttributeType type, bool normalize = false)
      : location{location}, type{type}, size{BufferAttributeToByteSize(type)}, offset{0}, normalize{normalize}
    {}
  };

  class BufferStructure
  {
    private:
      std::vector<BufferAttribute> attributes;
      uint vertexSize;

    public:
      BufferStructure() {}
      BufferStructure(const std::vector<BufferAttribute>& attributes) 
        : attributes{attributes}, vertexSize{0}
      {
        uint offset = 0;
        for(auto&& attribute : this->attributes)
        {
          attribute.offset = offset;
          offset += attribute.size;
        }
        vertexSize = offset;
      }

      uint GetVertexSize() const
      {
        return vertexSize;
      }

      std::vector<BufferAttribute>::iterator begin() { return attributes.begin(); }
      std::vector<BufferAttribute>::iterator end() { return attributes.end(); }
      std::vector<BufferAttribute>::const_iterator begin() const { return attributes.begin(); }
      std::vector<BufferAttribute>::const_iterator end() const { return attributes.end(); }
  };

  class VertexBuffer : public Buffer
  {
    private:
      BufferStructure structure;
    public:
      VertexBuffer(const void* data, uint size, BufferDrawType type);
      virtual ~VertexBuffer() {}

      void SetStructure(const BufferStructure& structure) { this->structure = structure; }
      const BufferStructure& GetStructure() const { return structure; }

      static Ref<VertexBuffer> CreateDynamic(const void* data, uint size);
      static Ref<VertexBuffer> CreateStatic(const void* data, uint size);
      static Ref<VertexBuffer> Create(const void* data, uint size, BufferDrawType type = BufferDrawType::STATIC);
  };
} 