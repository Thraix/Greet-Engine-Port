#pragma once

#include <common/Memory.h>
#include <internal/GreetGL.h>

namespace Greet {

  enum class BufferType
  {
    ARRAY = GL_ARRAY_BUFFER,
    INDEX = GL_ELEMENT_ARRAY_BUFFER,
  };

  enum class BufferDrawType : uint
  {
    STATIC = GL_STATIC_DRAW,
    DYNAMIC = GL_DYNAMIC_DRAW
  };

  class Buffer
  {
    protected:
      uint id;
      uint dataSize;
      BufferType type;
      BufferDrawType drawType;

    protected:
      Buffer(uint dataSize, BufferType type, BufferDrawType drawType);

    public:
      virtual ~Buffer();
      void UpdateData(const void* data) const;
      void UpdateData(const void* data, uint dataSize);
      void* MapBuffer() const;
      void UnmapBuffer() const;

      void Enable() const;
      void Disable() const;

      inline uint GetDataSize() const { return dataSize; }

    public:
      static Ref<Buffer> Create(uint dataSize, BufferType type, BufferDrawType drawType);
  };
}
