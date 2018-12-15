#pragma once

#include <internal/GreetGL.h>

namespace Greet {
  class IndexBuffer
  {
    private:
      GLuint m_bufferID;
      GLuint m_count;

    public:
      IndexBuffer(GLushort* data, GLsizei count);
      IndexBuffer(GLuint* data, GLsizei count);
      ~IndexBuffer();
      void Enable() const;
      void Disable() const;

      inline GLuint GetCount() const { return m_count; }

  };
}
