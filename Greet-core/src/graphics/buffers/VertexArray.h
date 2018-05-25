#pragma once

#include <internal/GreetGL.h>
#include "Buffer.h"
#include <vector>
#include <internal/GreetTypes.h>

namespace Greet {

	class VertexArray
	{
	private:
		GLuint m_arrayID;
		std::vector<Buffer*> m_buffers;
	public:
		VertexArray();
		~VertexArray();
		void AddBuffer(Buffer* buffer,GLuint index);
		void Enable() const;
		void Disable() const;
	};

}
