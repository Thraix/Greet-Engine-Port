#include "VertexArray.h"

namespace Greet {

	VertexArray::VertexArray()
	{
		GLCall(glGenVertexArrays(1, &m_arrayID));
	}

	VertexArray::~VertexArray()
	{
		for (uint i = 0; i < m_buffers.size(); i++)
			delete m_buffers[i];
		GLCall(glDeleteVertexArrays(1, &m_arrayID));
	}

	void VertexArray::AddBuffer(Buffer* buffer, GLuint index)
	{
		Enable();
		buffer->Enable();

		GLCall(glEnableVertexAttribArray(index));
		GLCall(glVertexAttribPointer(index, buffer->GetComponentCount(), GL_FLOAT,GL_FALSE, 0, 0));

		buffer->Disable();
		Disable();

		m_buffers.push_back(buffer);
	}

	void VertexArray::Enable() const
	{
		GLCall(glBindVertexArray(m_arrayID));
	}

	void VertexArray::Disable() const
	{
		GLCall(glBindVertexArray(0));
	}

}