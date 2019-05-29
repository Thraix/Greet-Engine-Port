#include "Shader.h"
#include <graphics/shaders/ShaderFactory.h>
#include <utils/ErrorHandle.h>
#include <utils/FileUtils.h>
#include <fstream>
#include <sstream>
#include <utils/HotSwapping.h>

namespace Greet {

  void ShaderDeleter::operator()(uint* id)
  {
    Log::Info(*id);
    GLCall(glDeleteProgram(*id));
    delete id;
  }

  Shader::Shader(const std::string& filename)
    : m_shaderID{new uint{Load(filename)}}, uniforms{GenUniforms(*m_shaderID.get())}
  {
    Log::Info(filename);
    hotswap = std::make_optional(HotSwapping::AddHotswapResource(this, filename));
    if(!hotswap.has_value())
      Log::Error("Optional has no value after set");
    Log::Info(*m_shaderID.get());
  }

  Shader::Shader(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc)
    : m_shaderID{new uint{Load(vertSrc, fragSrc, geomSrc, true)}}, uniforms{GenUniforms(*m_shaderID.get())}
  {

  }

  Shader::Shader(const std::string& vertSrc, const std::string& fragSrc)
    : m_shaderID{new uint{Load(vertSrc,fragSrc)}}, uniforms{GenUniforms(*m_shaderID.get())}
  {

  }

  Shader::Shader(Shader&& shader)
    : m_shaderID{std::move(shader.m_shaderID)}, hotswap{std::move(shader.hotswap)}, uniforms{std::move(shader.uniforms)}
  {
    Log::Info("Moved address: ", this);
    Log::Info("Uniforms size after move: ", uniforms.size());
    if(hotswap.has_value())
    {
      (*hotswap)->second.MoveResource(this);
      Log::Info("Moved hotswap resource");
    }
  }

  Shader& Shader::operator=(Shader&& shader)
  {
    m_shaderID = std::move(shader.m_shaderID);
    uniforms = std::move(shader.uniforms);
    hotswap = std::move(shader.hotswap);

    if(hotswap.has_value())
    {
      (*hotswap)->second.MoveResource(this);
      Log::Info("Moved hotswap resource");
    }

    return *this;
  }

  Shader::~Shader()
  {
    Log::Info(this);
  }

  void Shader::ReloadResource(const std::string& filename)
  {
    Log::Info("Reload address: ", this);
    if(m_shaderID)
      m_shaderID.reset(new uint{Load(filename)});
    else
      Log::Error("Invalid pointer");
  }

  uint Shader::Load(const std::string& filename)
  {
    const static uint VERTEX = 0;
    const static uint FRAGMENT = 1;
    const static uint GEOMETRY = 2;
    std::stringstream ss[3];
    std::ifstream file(filename);
    if (!file.good())
    {
      Log::Error("Shader::FromFile Couldn't find shader in path \'", filename, "\'");
      return Load(ShaderFactory::default_shader_vert, ShaderFactory::default_shader_frag);
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
    return Load(ss[VERTEX].str(), ss[FRAGMENT].str(), ss[GEOMETRY].str(), !ss[GEOMETRY].str().empty());
  }

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

  std::map<std::string, int> Shader::GenUniforms(const uint program)
  {
    std::map<std::string, int> uniforms;
    GLint numActiveUniforms = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
    std::vector<GLchar> nameData(256);
    for(int i= 0; i< numActiveUniforms; ++i)
    {
      GLint arraySize = 0;
      GLenum type = 0;
      GLsizei actualLength = 0;
      glGetActiveUniform(program, i, nameData.size(), &actualLength, &arraySize, &type, &nameData[0]);

      // For some reason arrays return 'arrayName[0]'
      for(int i = 0;i<actualLength;i++)
      {
        if(nameData[i] == '[')
        {
          nameData[i] = '\0';
          break;
        }
      }
      std::string name(nameData.data());
      uniforms.emplace(name, glGetUniformLocation(program, name.c_str()));
      Log::Info("Found uniform \'", name, "\' ", arraySize);
    }
    return uniforms;
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
      else
      {
        Log::Error("Failed to compile unknown shader!\n", &error[0]);
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

  int Shader::GetUniformLocation(const std::string& name) const
  {
    auto it = uniforms.find(name);
    if(it != uniforms.end())
      return it->second;
    return -1;
  }

  void Shader::SetUniformBoolean(const std::string& name, bool value) const
  {
    GLCall(glUniform1f(GetUniformLocation(name), value ? 1.0f : 0.0f));
  }

  void Shader::SetUniform1f(const std::string& name, float value) const
  {
    GLCall(glUniform1f(GetUniformLocation(name), value));
  }

  void Shader::SetUniform1fv(const std::string& name, uint count, float* value) const
  {
    GLCall(glUniform1fv(GetUniformLocation(name), count, value));
  }

  void Shader::SetUniform1i(const std::string& name, int value) const
  {
    GLCall(glUniform1i(GetUniformLocation(name), value));
  }

  void Shader::SetUniform1iv(const std::string& name, uint count, int* value) const
  {
    GLCall(glUniform1iv(GetUniformLocation(name), count, value));
  }

  void Shader::SetUniform1ui(const std::string& name, uint value) const
  {
    GLCall(glUniform1ui(GetUniformLocation(name), value));
  }

  void Shader::SetUniform1uiv(const std::string& name, uint count, uint* value) const
  {
    GLCall(glUniform1uiv(GetUniformLocation(name), count, value));
  }

  void Shader::SetUniform2f(const std::string& name, const Vec2 &value) const
  {
    GLCall(glUniform2f(GetUniformLocation(name), value.x, value.y));
  }

  void Shader::SetUniform3f(const std::string& name, const Vec3<float>& value) const
  {
    GLCall(glUniform3f(GetUniformLocation(name), value.x, value.y, value.z));
  }

  void Shader::SetUniform4f(const std::string& name, const Vec4 &value) const
  {
    GLCall(glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w));
  }

  void Shader::SetUniformMat3(const std::string& name, const Mat3 &value) const
  {
    GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, value.elements));
  }

  void Shader::SetUniformMat4(const std::string& name, const Mat4 &value) const
  {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value.elements));
  }

  bool Shader::operator<(const Shader& s)
  {
    return *m_shaderID.get() < *s.m_shaderID.get();
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
    return Shader(shaderPath);
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
