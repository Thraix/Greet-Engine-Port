#pragma once
#if 0
#include <graphics/shaders/uniforms/Uniform.h>
#include <set>

namespace Greet {

  class Uniformable
  {
    public:
      std::set<Uniform, UniformCompare> m_uniforms;

      Uniform& GetUniform(const std::string& location)
      {
        return *(m_uniforms.find(location));
      }

      void AddUniform(const Uniform& uniform)
      {
        m_uniforms.emplace(uniform);
      }

      void RemoveUniform(const std::string& location)
      {
        auto it = m_uniforms.find(location);
        if (it == m_uniforms.end())
        {
          Log::Warning("Could not remove Uniform since it doesn't exist: ", location);
          return;
        }
        m_uniforms.erase(it);
      }
    protected:
      void BindUniforms(Shader* shader)
      {
        for (auto it = m_uniforms.begin(); it != m_uniforms.end(); ++it)
        {
          (*it).BindUniform(shader);
        }
      }
  };
}
#endif
