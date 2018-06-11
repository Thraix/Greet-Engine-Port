#include "OpenGLObjectHandler.h"

#include <internal/GreetGL.h>

namespace Greet {

	std::map < OpenGLType, std::map<uint, uint>> OpenGLObjectHandler::m_openGLObjects;


	uint OpenGLObjectHandler::CreateOpenGLObject(const OpenGLType& type)
	{
		uint id;
		switch (type)
		{
		case SHADER:
			id = glCreateProgram();
			break;
		case TEXTURE:
			GLCall(glGenTextures(1, &id));
			break;
		case BUFFER:
			GLCall(glGenBuffers(1, &id));
			break;
		case VERTEX_ARRAY:
			GLCall(glGenVertexArrays(1, &id));
      break;
		default:
      // Incase I forget to add a switch for future objects
			Log::Warning("No such OpenGLType at create: ", OpenGLTypeName(type));
		}

		if (id == 0)
			Log::Error("OpenGLObject could not be created.");

		auto it = m_openGLObjects.find(type);

		// Add the type to the map if it doesn't exist
		if (it == m_openGLObjects.end())
		{
			auto pair = m_openGLObjects.emplace(type, std::map<uint,uint>());
			it = pair.first;
		}

		auto pair = it->second.emplace(id,1);
		if(!pair.second)
			Log::Error("OpenGLObject created a duplicate id.");
		return id;
	}

	void OpenGLObjectHandler::CopyOpenGLObject(const OpenGLType& type, uint id)
	{
		auto itType = m_openGLObjects.find(type);
		if (itType == m_openGLObjects.end())
		{
			Log::Warning("Copying from non-existing OpenGLType");
			return;
		}
		auto itObj = itType->second.find(id);
		if (itObj == itType->second.end())
		{
			Log::Warning("Copying non-existing OpenGLObject");
			Log::Warning("Copying non-existing OpenGLObject (type=",OpenGLTypeName(type)," id=",id,")");
			return;
		}
		itObj->second++;
	}

	void OpenGLObjectHandler::DestroyOpenGLObject(const OpenGLType& type, uint id)
	{
		auto itType = m_openGLObjects.find(type);
		if (itType == m_openGLObjects.end())
		{
			Log::Warning("Deleting from non-existing OpenGLType");
			return;
		}
		auto itObj = itType->second.find(id);
		if (itObj == itType->second.end())
		{
			Log::Warning("Deleting non-existing OpenGLObject (type=",OpenGLTypeName(type)," id=",id,")");
			return;
		}
		itObj->second--;
		if (itObj->second == 0)
		{
			DeleteOpenGLObject(type, itType, itObj);
		}
	}

	void OpenGLObjectHandler::DeleteOpenGLObject(const OpenGLType& type, const std::map<OpenGLType, std::map<uint, uint>>::iterator& typeIterator, const std::map<uint, uint>::iterator& objIterator)
	{
		switch (type)
		{
		case SHADER:
			GLCall(glDeleteProgram(objIterator->first));
			break;
		case TEXTURE:
			GLCall(glDeleteTextures(1, &objIterator->first));
			break;
		case BUFFER:
			GLCall(glDeleteBuffers(1, &objIterator->first));
			break;
		case VERTEX_ARRAY:
			GLCall(glDeleteVertexArrays(1, &objIterator->first));
      break;
		default:
      // Incase I forget to add a switch for future objects
			Log::Warning("No such OpenGLType at delete: ", type);
		}

		// Remove the object from the map.
		typeIterator->second.erase(objIterator);
	}
  
  std::string OpenGLObjectHandler::OpenGLTypeName(OpenGLType type)
  {
		switch (type)
		{
		case SHADER:
      return "Shader";
    case TEXTURE:
      return "Texture";
    case BUFFER:
      return "Buffer";
    case VERTEX_ARRAY:
      return "Vertex array";
    default:
      // Incase I forget to add a switch for future objects
      return "No name";
		}
  
  }
}
