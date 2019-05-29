#pragma once

#include <string>
#include <set>
#include <math/Maths.h>
#include <memory>
#include <functional>
#include <utils/Utils.h>
#include <utils/FileUtils.h>
#include <utils/HotSwapping.h>

namespace Greet {

  struct UniformData
  {
    UniformData(const std::string& name, int arraySize, uint type)
      : name{name}, arraySize{arraySize}, type{type}
    {}
    std::string name;
    int arraySize;
    uint type;
  };

  struct ShaderDeleter final
  {
    void operator()(uint* id);
  };

  class Shader final
  {
    private:
      std::unique_ptr<uint, ShaderDeleter> m_shaderID;
      std::map<std::string, int> uniforms;
      std::optional<std::map<uint, HotswapResource>::iterator> hotswap;

    private:
      Shader(const std::string& filename);
      Shader(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc);
      Shader(const std::string& vertSrc, const std::string& fragSrc);

      std::array<std::stringstream,3> ReadFile(const std::string& filename);
      uint Load(const std::string& filename);
      uint Load(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc, bool hasGeometry);
      uint Load(const std::string& vertSrc, const std::string& fragSrc);
      uint CompileAttachShader(const uint program, const std::string& shaderSrc, const uint shaderType, bool safeFail);
      uint CompileShader(const uint program, const std::string& shaderSrc, const uint shaderType, bool safeFail);
      void AttachShader(const uint program, uint shader);
      int GetUniformLocation(const std::string& name) const;
      std::map<std::string, int> GetUniforms(const uint program);
      std::vector<UniformData> GetListOfUniforms(uint program);
      void MoveUniforms(uint program, uint oldProgram);

    public:
      Shader(Shader&&);
      Shader& operator=(Shader&&);
      ~Shader();

      void ReloadResource(const std::string& filename);
      void Enable() const;
      static void Disable();
      void BindAttributeOutput(uint attachmentId, const std::string& name) const;

      void SetUniformBoolean(const std::string& name, bool value) const;
      void SetUniform1f(const std::string& name, float value) const;
      void SetUniform1fv(const std::string& name, uint count, float* value) const;
      void SetUniform1i(const std::string& name, int value) const;
      void SetUniform1iv(const std::string& name, uint count, int* value) const;
      void SetUniform1ui(const std::string& name, uint value) const;
      void SetUniform1uiv(const std::string& name, uint count, uint* values) const;
      void SetUniform2f(const std::string& name, const Vec2& value) const;
      void SetUniform3f(const std::string& name, const Vec3<float>& value) const;
      void SetUniform4f(const std::string& name, const Vec4& value) const;
      void SetUniformMat3(const std::string& name, const Mat3 &value) const;
      void SetUniformMat4(const std::string& name, const Mat4 &value) const;
      bool operator<(const Shader& s);
    public:
      static Shader FromFile(const std::string& shaderPath);
      static Shader FromFile(const std::string& vertPath, const std::string& fragPath);
      static Shader FromFile(const std::string& vertPath, const std::string& fragPath, const std::string& geomPath);
      static Shader FromSource(const std::string& vertSrc, const std::string& fragSrc);
      static Shader FromSource(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc);
  };

}
