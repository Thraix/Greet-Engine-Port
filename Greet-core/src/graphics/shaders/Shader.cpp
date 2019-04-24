#include "Shader.h"
#include <graphics/shaders/ShaderFactory.h>
#include <utils/ErrorHandle.h>
#include <utils/FileUtils.h>
#include <fstream>
#include <sstream>

namespace Greet {

  void ShaderDeleter::operator()(uint* id)
  {
    glDeleteProgram(*id);
    delete id;
  }

  Shader::Shader(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc)
    : m_shaderID{new uint{Load(vertSrc, fragSrc, geomSrc, true)}}
  {}

  Shader::Shader(const std::string& vertSrc, const std::string& fragSrc)
    : m_shaderID{new uint{Load(vertSrc,fragSrc)}}
  {}

  uint Shader::Load(const std::string& vertSrc, const std::string& fragSrc)
  {
    return Load(vertSrc, fragSrc, "", false);
  }

  uint Shader::Load(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc, bool hasGeometry)
  {
    GLCall(uint program = glCreateProgram()); 

    uint shader = AttachShader(program, vertSrc, GL_VERTEX_SHADER);
    if (!shader)
      return shader;
    shader = AttachShader(program, fragSrc, GL_FRAGMENT_SHADER);
    if (!shader)
      return shader;
    if (hasGeometry)
    {
      // If this fails just ignore it
      AttachShader(program, geomSrc, GL_GEOMETRY_SHADER);
    }
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));


    return program;
  }

  uint Shader::AttachShader(const uint program, const std::string& shaderSrc, const uint shaderType)
  {
    GLCall(uint shader = glCreateShader(shaderType));
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
        Log::Info(shaderSrc);
        ErrorHandle::SetErrorCode(GREET_ERROR_SHADER_FRAGMENT);
        // Should never fail
        GLCall(glShaderSource(shader, 1, &ShaderFactory::default_shader_frag, NULL));
      }
      else if (shaderType == GL_VERTEX_SHADER)
      {
        Log::Error("Failed to compile vertex Shader!\n", &error[0]);
        ErrorHandle::SetErrorCode(GREET_ERROR_SHADER_VERTEX);
        // Should never fail
        GLCall(glShaderSource(shader, 1, &ShaderFactory::default_shader_vert, NULL));
      }
      else if (shaderType == GL_GEOMETRY_SHADER)
      {
        Log::Error("Failed to compile geometry Shader!\n", &error[0]);
        ErrorHandle::SetErrorCode(GREET_ERROR_SHADER_GEOMETRY);
        return 0;
      }
      GLCall(glCompileShader(shader));
    }	
    GLCall(glAttachShader(program, shader));
    GLCall(glDeleteShader(shader));
    return 1;
  }

  void Shader::BindAttributeOutput(uint attachmentId, const std::string& name) const
  {
    GLCall(glBindFragDataLocation(*m_shaderID,attachmentId,name.c_str()));
  }

  uint Shader::GetUniformLocation(const char *name) const
  {
    GLCall(uint location = glGetUniformLocation(*m_shaderID,name));
    return location;
  }

  void Shader::SetUniformBoolean(const char *name, bool value) const
  {
    GLCall(glUniform1f(GetUniformLocation(name), value ? 1.0f : 0.0f));
  }

  void Shader::SetUniform1f(const char *name, float value) const
  {
    GLCall(glUniform1f(GetUniformLocation(name), value));
  }

  void Shader::SetUniform1fv(const char *name, uint count, float* value) const
  {
    GLCall(glUniform1fv(GetUniformLocation(name), count, value));
  }

  void Shader::SetUniform1i(const char *name, int value) const
  {
    GLCall(glUniform1i(GetUniformLocation(name), value));
  }

  void Shader::SetUniform1iv(const char *name, uint count, int* value) const
  {
    GLCall(glUniform1iv(GetUniformLocation(name), count, value));
  }

  void Shader::SetUniform1ui(const char *name, uint value) const
  {
    GLCall(glUniform1ui(GetUniformLocation(name), value));
  }

  void Shader::SetUniform1uiv(const char *name, uint count, uint* value) const
  {
    GLCall(glUniform1uiv(GetUniformLocation(name), count, value));
  }

  void Shader::SetUniform2f(const char *name, const Vec2 &value) const
  {
    GLCall(glUniform2f(GetUniformLocation(name), value.x, value.y));
  }

  void Shader::SetUniform3f(const char *name, const Vec3<float>& value) const
  {
    GLCall(glUniform3f(GetUniformLocation(name), value.x, value.y, value.z));
  }

  void Shader::SetUniform4f(const char *name, const Vec4 &value) const
  {
    GLCall(glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w));
  }

  void Shader::SetUniformMat3(const char *name, const Mat3 &value) const
  {
    GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, value.elements));
  }

  void Shader::SetUniformMat4(const char *name, const Mat4 &value) const
  {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value.elements));
  }

  void Shader::Enable() const
  {
    GLCall(glUseProgram(*m_shaderID.get()));
  }

  void Shader::Disable()
  {
    GLCall(glUseProgram(0));
  }

  Shader Shader::FromFile(const std::string& shaderPath)
  {
    const static uint VERTEX = 0;
    const static uint FRAGMENT = 1;
    const static uint GEOMETRY = 2;
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


  Shader Shader::FromFile(const std::string& vertPath, const std::string& fragPath, const std::string& geomPath)
  {	
    std::string vertSourceString = FileUtils::read_file(vertPath.c_str());
    std::string fragSourceString = FileUtils::read_file(fragPath.c_str());
    std::string geomSourceString = FileUtils::read_file(geomPath.c_str());
    return Shader(vertSourceString,fragSourceString,geomSourceString);
  }

  Shader Shader::FromSource(const std::string& vertSrc, const std::string& fragSrc)
  {
    return Shader(vertSrc, fragSrc);
  }

  Shader Shader::FromSource(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc)
  {
    return Shader(vertSrc, fragSrc, geomSrc);
  }
}
