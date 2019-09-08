#pragma once

#include <graphics/textures/CubeMap.h>
#include <graphics/cameras/Camera.h>
#include <graphics/shaders/Shader.h>
#include <graphics/models/Mesh.h>

namespace Greet {

  class Skybox
  {
    private:
      const CubeMap& m_map;
      Shader m_shader;
      Mesh* m_mesh;
    public:
      Skybox(const CubeMap& map);
      Skybox(const CubeMap& map, Shader&& shader);
      const CubeMap& GetCubeMap() const { return m_map; };
      virtual ~Skybox();
      void Render(const Mat4& projectionMatrix, const Mat4& viewMatrix) const;
      void Render(const Camera& camera) const;
  };
}
