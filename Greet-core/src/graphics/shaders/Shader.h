#pragma once

#include <logging/Log.h>
#include <internal/GreetGL.h>
#include <math/Maths.h>
#include <utils/FileUtils.h>
#include <iostream>
#include <vector>
#include <utils/ErrorHandle.h>
#include <sstream>
#include <fstream>

namespace Greet {
	class Shader
	{
	private:
		GLuint m_shaderID;

	private:
		GLuint Load(const std::string& geomSrc, const std::string& vertSrc, const std::string& fragSrc, bool hasGeometry);
		GLuint Load(const std::string& vertSrc, const std::string& fragSrc);
		GLuint AttachShader(const GLuint program, const std::string& shaderSrc, const GLuint shaderType);
		GLuint GetUniformLocation(const GLchar *name) const;

		Shader(const std::string& geomSrc, const std::string& vertSrc, const std::string& fragSrc);
		Shader(const std::string& vertSrc, const std::string& fragSrc);
	public:

		virtual ~Shader();
		void Enable() const;
		static void Disable();
		void BindAttributeOutput(uint attachmentId, const std::string& name);

		void SetUniformBoolean(const GLchar *name, bool value) const;
		void SetUniform1f(const GLchar *name, float value) const;
		void SetUniform1fv(const GLchar *name, uint count, float* value) const;
		void SetUniform1i(const GLchar *name, int value) const;
		void SetUniform1iv(const GLchar *name, uint count, int* value) const;
		void SetUniform1ui(const GLchar *name, uint value) const;
		void SetUniform1uiv(const GLchar *name, uint count, uint* values) const;
		void SetUniform2f(const GLchar *name, const Vec2 &value) const;
		void SetUniform3f(const GLchar *name, const Vec3 &value) const;
		void SetUniform4f(const GLchar *name, const Vec4 &value) const;
		void SetUniformMat3(const GLchar *name, const Mat3 &value) const;
		void SetUniformMat4(const GLchar *name, const Mat4 &value) const;
	public:
		static Shader* FromFile(const std::string& shaderPath);
		static Shader* FromFile(const std::string& vertPath, const std::string& fragPath);
		static Shader* FromFile(const std::string& geomPath, const std::string& vertPath, const std::string& fragPath);
		static Shader* FromSource(const std::string& vertSrc, const std::string& fragSrc);
		static Shader* FromSource(const std::string& geomSrc, const std::string& vertSrc, const std::string& fragSrc);
	};

}
