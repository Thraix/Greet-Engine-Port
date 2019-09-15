#include "VertexArray.h"

#include <internal/GreetGL.h>

namespace Greet {

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

  void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer)
  {
    const BufferStructure& structure = buffer->GetStructure();
    for(auto&& attribute : structure)
    {
      GLCall(glEnableVertexAttribArray(attribute.location));
      GLCall(glVertexAttribPointer(
            attribute.location,
            BufferAttributeToCount(attribute.type),
            BufferAttributeToGLPrimitive(attribute.type),
            attribute.normalize ? GL_TRUE : GL_FALSE,
            structure.GetVertexSize(),
            (const void*)attribute.offset
            ));
    }
    buffers.push_back(buffer);
  }

  void VertexArray::Enable() const
  {
    GLCall(glBindVertexArray(id));
  }

  void VertexArray::Disable()
  {
    GLCall(glBindVertexArray(0));
  }

  Ref<VertexArray> VertexArray::Create()
  {
    return std::shared_ptr<VertexArray>(new VertexArray());
  }
}
