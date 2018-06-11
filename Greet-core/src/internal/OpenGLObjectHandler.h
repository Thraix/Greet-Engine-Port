#pragma once

#include <internal/GreetTypes.h>

#include <map>

namespace Greet {

	enum OpenGLType
	{
		SHADER, TEXTURE, BUFFER, VERTEX_ARRAY
	};

	class OpenGLObjectHandler
	{

	private:
		static std::map < OpenGLType, std::map<uint, uint>> m_openGLObjects; // map<type, map<id, count> >
    static std::string OpenGLTypeName(OpenGLType type);

	public:
		// Creates the object.
		static uint CreateOpenGLObject(const OpenGLType& type);
		// Adds one to the object counter.
		static void CopyOpenGLObject(const OpenGLType& type, uint id);
		// Removes one from the object counter.
		static void DestroyOpenGLObject(const OpenGLType& type, uint id);

	private:
		// Fully deletes the object from the map.
		static void DeleteOpenGLObject(const OpenGLType& type, const std::map<OpenGLType, std::map<uint, uint>>::iterator& typeIterator, const std::map<uint, uint>::iterator& objIterator);
	};
}
