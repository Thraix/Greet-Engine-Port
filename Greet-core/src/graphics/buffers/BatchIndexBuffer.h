#pragma once

#include <internal/GreetGL.h>

namespace Greet {
  class BatchIndexBuffer
  {
    private:
      GLuint m_bufferID;
      GLsizei m_count;
    public:
      BatchIndexBuffer(GLushort* data, GLsizei amount);
      BatchIndexBuffer(GLuint* data, GLsizei amount);

      ~BatchIndexBuffer();
      void Enable() const;
      void Disable() const;
      void AddCount(GLuint count){ m_count += count; }
      void ResetCount(){ m_count = 0; }
      inline GLsizei GetCount() const { return m_count; }
  };
}
