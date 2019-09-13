#pragma once

#include <vector>
#include <common/Types.h>
#include <common/Memory.h>

namespace Greet {

  struct VertexArrayDeleter
  {
    void operator()(uint* id);
  };

  class VertexArray final 
  {
    private:
      uint id;
    private:
      VertexArray();
    public:
      ~VertexArray();

      VertexArray(VertexArray&&) = default;
      VertexArray& operator=(VertexArray&&) = default;

      void Enable() const;
    public:
      static void Disable();

      static Ref<VertexArray> CreateVertexArray();
  };

}
