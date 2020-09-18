#pragma once

#include <math/Maths.h>
#include <graphics/shaders/Shader.h>
#include <string.h>

namespace Greet {
  class Light
  {
    public:
      Vec3f position;
      uint color;

    public:
      Light(const Vec3f& position, uint color) : position(position) , color(color) {}

      void SetToUniform(const Ref<Shader>& shader, const std::string& prefix)
      {
        shader->SetUniform3f(("u" + prefix + "Position").c_str(), position);
        Color c{color};
        shader->SetUniformColor3(("u" + prefix + "Color").c_str(), color);
      }

  };
}
