#include "VertexArray.h"

#include <internal/GreetGL.h>

namespace Greet {

  void VertexArrayDeleter::operator()(uint* id)
  {
    glDeleteVertexArrays(1, id);
    delete id;
  }

  VertexArray::VertexArray()
    : id{0}
  {
    GLCall(glGenVertexArrays(1, &id));
    Enable();
  }
  VertexArray::~VertexArray()
  {
    glDeleteVertexArrays(1, &id);
  }

  void VertexArray::Enable() const
  {
    GLCall(glBindVertexArray(id));
  }

  void VertexArray::Disable()
  {
    GLCall(glBindVertexArray(0));
  }

  Ref<VertexArray> VertexArray::CreateVertexArray()
  {
    return std::shared_ptr<VertexArray>(new VertexArray());
  }
}
