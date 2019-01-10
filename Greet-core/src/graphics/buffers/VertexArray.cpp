#include "VertexArray.h"

#include <internal/GreetGL.h>

namespace Greet {

  void VertexArrayDeleter::operator()(uint* id)
  {
    glDeleteVertexArrays(1, id);
    delete id;
  }

  VertexArray::VertexArray()
    : id{new uint{0}}
  {
    GLCall(glGenVertexArrays(1, id.get()));
    Enable();
  }

  void VertexArray::Enable() const
  {
    GLCall(glBindVertexArray(*id));
  }

  void VertexArray::Disable()
  {
    GLCall(glBindVertexArray(0));
  }

}
