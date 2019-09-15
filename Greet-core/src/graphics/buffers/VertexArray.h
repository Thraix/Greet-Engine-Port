#pragma once

#include <vector>
#include <common/Types.h>
#include <common/Memory.h>
#include <graphics/buffers/VertexBuffer.h>

namespace Greet {

  class VertexArray final 
  {
    private:
      uint id;
      std::vector<Ref<VertexBuffer>> buffers;
    private:
      VertexArray();
    public:
      ~VertexArray();

      void Enable() const;
      void AddVertexBuffer(const Ref<VertexBuffer>& buffer);
    public:
      static void Disable();

      static Ref<VertexArray> Create();
  };

}
