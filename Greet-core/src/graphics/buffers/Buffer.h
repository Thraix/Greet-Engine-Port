#pragma once

#include <memory>
#include <internal/GreetGL.h>

namespace Greet {

  struct BufferDeleter final
  {
    void operator()(uint* id);
  };

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

  class Buffer final
  {
    private:
      std::unique_ptr<uint> id;
      uint dataSize;
      BufferType type;
      BufferDrawType drawType;

    public:
      Buffer(uint dataSize, BufferType type, BufferDrawType drawType);

      Buffer(Buffer&&) = default;
      Buffer& operator=(Buffer&&) = default;

      void UpdateData(void* data) const;

      void Enable() const;
      void Disable() const;

      inline uint GetDataSize() const { return dataSize; }

  };
}
