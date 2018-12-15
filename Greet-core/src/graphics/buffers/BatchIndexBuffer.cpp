#include "BatchIndexBuffer.h"

namespace Greet {
  BatchIndexBuffer::BatchIndexBuffer(GLushort *data, GLsizei amount)
    : m_count(0)
  {
    GLCall(glGenBuffers(1, &m_bufferID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, amount * sizeof(GLshort), data, GL_DYNAMIC_DRAW));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  }

  BatchIndexBuffer::BatchIndexBuffer(GLuint* data, GLsizei amount)
    : m_count(0)
  {
    GLCall(glGenBuffers(1, &m_bufferID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, amount * sizeof(GLuint), data, GL_DYNAMIC_DRAW));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  }

  BatchIndexBuffer::~BatchIndexBuffer()
  {
    GLCall(glDeleteBuffers(1,&m_bufferID));
  }

  void BatchIndexBuffer::Enable() const
  {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID));
  }

  void BatchIndexBuffer::Disable() const
  {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  }
}
