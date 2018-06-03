#include "Shader.h"

#include <graphics/shaders/ShaderFactory.h>
#include <utils/ErrorHandle.h>
#include <utils/FileUtils.h>

#include <fstream>
#include <sstream>

#include <internal/OpenGLObjectHandler.h>


namespace Greet {

	Shader::Shader(const std::string& geomSrc, const std::string& vertSrc, const std::string& fragSrc)
	{
		m_shaderID = Load(geomSrc, vertSrc, fragSrc,true);
	}

	Shader::Shader(const std::string& vertSrc, const std::string& fragSrc)
	{
		m_shaderID = Load(vertSrc,fragSrc);
	}

	Shader::Shader(const Shader& shader)
	{
		operator=(shader);
	}

	Shader::~Shader()
	{
		OpenGLObjectHandler::DestroyOpenGLObject(OpenGLType::SHADER, m_shaderID);
	}

	Shader& Shader::operator=(const Shader& shader)
	{
		if (this != &shader)
		{
			OpenGLObjectHandler::CopyOpenGLObject(OpenGLType::SHADER, shader.m_shaderID);
			m_shaderID = shader.m_shaderID;
		}
		return *this;
	}

	GLuint Shader::Load(const std::string& vertSrc, const std::string& fragSrc)
	{
		return Load("", vertSrc, fragSrc, false);
	}

	GLuint Shader::Load(const std::string& geomSrc, const std::string& vertSrc, const std::string& fragSrc, bool hasGeometry)
	{
		uint program = OpenGLObjectHandler::CreateOpenGLObject(OpenGLType::SHADER);
		uint shader = AttachShader(program, vertSrc, GL_VERTEX_SHADER);
		if (shader != 0)
			return shader;
		shader = AttachShader(program, fragSrc, GL_FRAGMENT_SHADER);
		if (shader != 0)
			return shader;
		if (hasGeometry)
		{
			AttachShader(program, geomSrc, GL_GEOMETRY_SHADER);
			if (shader != 0)
				return shader;
		}
		GLCall(glLinkProgram(program));
		GLCall(glValidateProgram(program));


		return program;
	}

	GLuint Shader::AttachShader(const GLuint program, const std::string& shaderSrc, const GLuint shaderType)
	{
		GLCall(GLuint shader = glCreateShader(shaderType));
		const char* src = shaderSrc.c_str();
		GLCall(glShaderSource(shader, 1, &src, NULL));
		GLCall(glCompileShader(shader));

		GLint resultFrag;
		GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &resultFrag));
		if (resultFrag == GL_FALSE)
		{
			GLint length;
			GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
			std::vector<char> error(length);
			GLCall(glGetShaderInfoLog(shader, length, &length, &error[0]));
			if (shaderType == GL_FRAGMENT_SHADER)
			{
				Log::Error("Failed to compile fragment Shader!\n", &error[0]);
				ErrorHandle::SetErrorCode(GREET_ERROR_SHADER_FRAGMENT);
			}
			else if (shaderType == GL_VERTEX_SHADER)
			{
				Log::Error("Failed to compile vertex Shader!\n", &error[0]);
				ErrorHandle::SetErrorCode(GREET_ERROR_SHADER_VERTEX);
			}
			else if (shaderType == GL_GEOMETRY_SHADER)
			{
				Log::Error("Failed to compile geometry Shader!\n", &error[0]);
				ErrorHandle::SetErrorCode(GREET_ERROR_SHADER_GEOMETRY);
			}
			GLCall(glDeleteShader(shader));
			return ShaderFactory::DefaultShader().m_shaderID;
		}	
		GLCall(glAttachShader(program, shader));
		GLCall(glDeleteShader(shader));
		return 0;
	}

	void Shader::BindAttributeOutput(uint attachmentId, const std::string& name)
	{
		GLCall(glBindFragDataLocation(m_shaderID,attachmentId,name.c_str()));
	}

	GLuint Shader::GetUniformLocation(const GLchar *name) const
	{
		GLCall(uint location = glGetUniformLocation(m_shaderID,name));
		return location;
	}

	void Shader::SetUniformBoolean(const GLchar *name, bool value) const
	{
		GLCall(glUniform1f(GetUniformLocation(name), value ? 1.0f : 0.0f));
	}

	void Shader::SetUniform1f(const GLchar *name, float value) const
	{
		GLCall(glUniform1f(GetUniformLocation(name), value));
	}

	void Shader::SetUniform1fv(const GLchar *name, uint count, float* value) const
	{
		GLCall(glUniform1fv(GetUniformLocation(name), count, value));
	}

	void Shader::SetUniform1i(const GLchar *name, int value) const
	{
		GLCall(glUniform1i(GetUniformLocation(name), value));
	}

	void Shader::SetUniform1iv(const GLchar *name, uint count, int* value) const
	{
		GLCall(glUniform1iv(GetUniformLocation(name), count, value));
	}

	void Shader::SetUniform1ui(const GLchar *name, uint value) const
	{
		GLCall(glUniform1ui(GetUniformLocation(name), value));
	}

	void Shader::SetUniform1uiv(const GLchar *name, uint count, uint* value) const
	{
		GLCall(glUniform1uiv(GetUniformLocation(name), count, value));
	}

	void Shader::SetUniform2f(const GLchar *name, const Vec2 &value) const
	{
		GLCall(glUniform2f(GetUniformLocation(name), value.x, value.y));
	}

	void Shader::SetUniform3f(const GLchar *name, const Vec3 &value) const
	{
		GLCall(glUniform3f(GetUniformLocation(name), value.x, value.y, value.z));
	}

	void Shader::SetUniform4f(const GLchar *name, const Vec4 &value) const
	{
		GLCall(glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w));
	}

	void Shader::SetUniformMat3(const GLchar *name, const Mat3 &value) const
	{
		GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, value.elements));
	}

	void Shader::SetUniformMat4(const GLchar *name, const Mat4 &value) const
	{
		GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value.elements));
	}

	void Shader::Enable() const
	{
		GLCall(glUseProgram(m_shaderID));
	}

	void Shader::Disable()
	{
		GLCall(glUseProgram(0));
	}

	Shader Shader::FromFile(const std::string& shaderPath)
	{
		const uint VERTEX = 0;
		const uint FRAGMENT = 1;
		const uint GEOMETRY = 2;
		std::stringstream ss[3];
		std::ifstream file(shaderPath);
		if (!file.good())
		{
			Log::Error("Shader::FromFile Couldn't find shader in path \'", shaderPath, "\'");
			return ShaderFactory::DefaultShader();
		}
		std::string line;
		uint shader = VERTEX;
		while (std::getline(file, line))
		{
			if (line == "//vertex")
				shader = VERTEX;
			else if (line == "//fragment")
				shader = FRAGMENT;
			else if (line == "//geometry")
				shader = GEOMETRY;
			else
				ss[shader] << line << std::endl;
		}
		if (ss[2].str().empty())
			return Shader(ss[VERTEX].str(), ss[FRAGMENT].str());
		else
			return Shader(ss[VERTEX].str(), ss[FRAGMENT].str(), ss[GEOMETRY].str());
	}

	Shader Shader::FromFile(const std::string& vertPath, const std::string& fragPath)
	{	
		std::string vertSourceString = FileUtils::read_file(vertPath.c_str());
		std::string fragSourceString = FileUtils::read_file(fragPath.c_str());
		return Shader(vertSourceString,fragSourceString);
	}


	Shader Shader::FromFile(const std::string& geomPath, const std::string& vertPath, const std::string& fragPath)
	{	
		std::string vertSourceString = FileUtils::read_file(vertPath.c_str());
		std::string fragSourceString = FileUtils::read_file(fragPath.c_str());
		std::string geomSourceString = FileUtils::read_file(geomPath.c_str());
		return Shader(geomSourceString, vertSourceString,fragSourceString);
	}

	Shader Shader::FromSource(const std::string& vertSrc, const std::string& fragSrc)
	{
		return Shader(vertSrc, fragSrc);
	}

	Shader Shader::FromSource(const std::string& geomSrc, const std::string& vertSrc, const std::string& fragSrc)
	{
		return Shader(vertSrc, fragSrc);
	}
}