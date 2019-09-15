#pragma once

#include <vector>
#include <common/Types.h>
#include <common/Memory.h>
#include <graphics/buffers/VertexBuffer.h>

namespace Greet {

  enum class DrawType
  {
    TRIANGLES = GL_TRIANGLES, LINES = GL_LINES
  };

  class VertexArray final 
  {
    private:
      uint id;
      std::vector<Ref<VertexBuffer>> buffers;
      Ref<Buffer> indexBuffer;
    private:
      VertexArray();
    public:
      ~VertexArray();

      void Enable() const;
      void Disable();
      void AddVertexBuffer(const Ref<VertexBuffer>& buffer);
      void SetIndexBuffer(const Ref<Buffer>& indexBuffer);

      void Render(DrawType type, uint count);

    public:
      static Ref<VertexArray> Create();
  };

}
