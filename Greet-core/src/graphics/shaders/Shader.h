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

  struct ShaderDeleter final
  {
    void operator()(uint* id);
  };

  class Shader final
  {
    private:
      std::unique_ptr<uint, ShaderDeleter> m_shaderID;
      std::optional<std::map<uint, HotswapResource>::iterator> hotswap;

    private:
      Shader(const std::string& filename);
      Shader(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc);
      Shader(const std::string& vertSrc, const std::string& fragSrc);

      uint Load(const std::string& filename);
      uint Load(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc, bool hasGeometry);
      uint Load(const std::string& vertSrc, const std::string& fragSrc);
      uint AttachShader(const uint program, const std::string& shaderSrc, const uint shaderType);
      uint GetUniformLocation(const char*name) const;

    public:
      Shader(Shader&&);
      Shader& operator=(Shader&&);
      ~Shader();

      void ReloadResource(const std::string& filename);
      void Enable() const;
      static void Disable();
      void BindAttributeOutput(uint attachmentId, const std::string& name) const;

      void SetUniformBoolean(const char *name, bool value) const;
      void SetUniform1f(const char *name, float value) const;
      void SetUniform1fv(const char *name, uint count, float* value) const;
      void SetUniform1i(const char *name, int value) const;
      void SetUniform1iv(const char *name, uint count, int* value) const;
      void SetUniform1ui(const char *name, uint value) const;
      void SetUniform1uiv(const char *name, uint count, uint* values) const;
      void SetUniform2f(const char *name, const Vec2& value) const;
      void SetUniform3f(const char *name, const Vec3<float>& value) const;
      void SetUniform4f(const char *name, const Vec4& value) const;
      void SetUniformMat3(const char *name, const Mat3 &value) const;
      void SetUniformMat4(const char *name, const Mat4 &value) const;
      bool operator<(const Shader& s);
    public:
      static Shader FromFile(const std::string& shaderPath);
      static Shader FromFile(const std::string& vertPath, const std::string& fragPath);
      static Shader FromFile(const std::string& vertPath, const std::string& fragPath, const std::string& geomPath);
      static Shader FromSource(const std::string& vertSrc, const std::string& fragSrc);
      static Shader FromSource(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc);
  };

}
