#include "Buffer.h"

#include <internal/GreetGL.h>

namespace Greet {

  void BufferDeleter::operator()(uint* id)
  {
    glDeleteBuffers(1, id);
    delete id;
  }

  Buffer::Buffer(uint dataSize, BufferType type, BufferDrawType drawType)
    : id{new uint{0}}, dataSize{dataSize}, type{type}, drawType{drawType}
  {
    GLCall(glGenBuffers(1, id.get()));
  }

  void Buffer::Enable() const
  {
    GLCall(glBindBuffer((uint)type, *id));
  }

  void Buffer::UpdateData(void* data) const
  {
    GLCall(glBufferData((uint)type, dataSize, data, (uint)drawType));
  }

  void Buffer::Disable() const
  {
    GLCall(glBindBuffer((uint)type, 0));
  }
}
