#pragma once
#if 0
#include <graphics/shaders/Shader.h>

namespace  Greet {

  struct UniformCompare
  {
    using is_transparent = void;
    bool operator()(const Uniform& u1, const Uniform& u2)
    {
      return u1.location < u2.location;
    }

    bool operator()(const Uniform& u, const std::string& s)
    {
      return u.location < s;
    }

    bool operator()(const std::string& s, const Uniform& u)
    {
      return s < u.location;
    }
  };

  struct Uniform
  {
    public:
      std::string location;
    public:
      Uniform(const std::string& location) : location(location) {}
      virtual void BindUniform(Shader* shader) const = 0;
  };

  struct Uniform1f : public Uniform
  {
    float value;
    Uniform1f(const std::string& location) : Uniform(location) {}
    Uniform1f(const std::string& location, float value) : Uniform(location), value(value) {}
    void BindUniform(Shader* shader) const override {
      shader->SetUniform1f(location.c_str(), value);
    }
  };

  struct Uniform2f : public Uniform
  {
    Vec2 value;
    Uniform2f(const std::string& location) : Uniform(location) {}
    Uniform2f(const std::string& location, Vec2 value) : Uniform(location), value(value) {}
    void BindUniform(Shader* shader)  const override {
      shader->SetUniform2f(location.c_str(), value);
    }
  };

  struct Uniform3f : public Uniform
  {
    Vec3 value;
    Uniform3f(const std::string& location) : Uniform(location) {}
    Uniform3f(const std::string& location, Vec3 value) : Uniform(location), value(value) {}
    void BindUniform(Shader* shader) const override {
      shader->SetUniform3f(location.c_str(), value);
    }
  };

  struct Uniform4f : public Uniform
  {
    Vec4 value;
    Uniform4f(const std::string& location) : Uniform(location) {}
    Uniform4f(const std::string& location, Vec4 value) : Uniform(location), value(value) {}
    void BindUniform(Shader* shader) const override {
      shader->SetUniform4f(location.c_str(), value);
    }
  };

  struct UniformMat3 : public Uniform
  {
    Mat3 value;
    UniformMat3(const std::string& location) : Uniform(location) {}
    UniformMat3(const std::string& location, Mat3 value) : Uniform(location), value(value) {}
    void BindUniform(Shader* shader) const override {
      shader->SetUniformMat3(location.c_str(), value);
    }
  };

  struct UniformMat4 : public Uniform
  {
    Mat4 value;
    UniformMat4(const std::string& location) : Uniform(location) {}
    UniformMat4(const std::string& location, Mat4 value) : Uniform(location), value(value) {}
    void BindUniform(Shader* shader) const override {
      shader->SetUniformMat4(location.c_str(), value);
    }
  };

  struct Uniform1i : public Uniform
  {
    int value;
    Uniform1i(const std::string& location) : Uniform(location) {}
    Uniform1i(const std::string& location, int value) : Uniform(location), value(value) {}
    void BindUniform(Shader* shader) const override {
      shader->SetUniform1i(location.c_str(), value);
    }
  };

  struct Uniform1ui : public Uniform
  {
    uint value;
    Uniform1ui(const std::string& location) : Uniform(location) {}
    Uniform1ui(const std::string& location, uint value) : Uniform(location), value(value) {}
    void BindUniform(Shader* shader) const override {
      shader->SetUniform1ui(location.c_str(), value);
    }
  };
}
#endif
