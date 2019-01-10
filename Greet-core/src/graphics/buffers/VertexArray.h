#pragma once

#include <vector>
#include <internal/GreetTypes.h>
#include <memory>

namespace Greet {

  struct VertexArrayDeleter
  {
    void operator()(uint* id);
  };

  class VertexArray final 
  {
    private:
      std::unique_ptr<uint, VertexArrayDeleter> id;
    public:
      VertexArray();

      VertexArray(VertexArray&&) = default;
      VertexArray& operator=(VertexArray&&) = default;

      void Enable() const;
      static void Disable();
  };

}
