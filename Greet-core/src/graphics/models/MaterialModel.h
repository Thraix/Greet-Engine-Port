#pragma once

#include <graphics/models/Material.h>
#include <graphics/models/Mesh.h>
#include <graphics/models/Camera.h>

namespace Greet {

  class Renderer3D;

  class MaterialModel
  {
    private:
      const Mesh* m_mesh;
      const Material* m_material;
    public:
      MaterialModel(const Mesh* mesh, const Material* material);
      virtual ~MaterialModel();

      void PreRender(const Renderer3D* renderer, const Camera* camera, const Mat4& transformationMatrix) const;
      void Render(const Renderer3D* renderer, const Camera* camera, const Mat4& transformationMatrix) const;
      void PostRender(const Renderer3D* renderer, const Camera* camera, const Mat4& transformationMatrix) const;

      void PreRender(const Renderer3D* renderer, const Camera* camera) const;
      void Render(const Renderer3D* renderer, const Camera* camera) const;
      void PostRender(const Renderer3D* renderer, const Camera* camera) const;

      inline const Material& GetMaterial() const { return *m_material; }
      inline const Mesh& GetMesh() const { return *m_mesh; }
      friend bool operator < (const MaterialModel & lhs, const MaterialModel & rhs)
      {
        if(lhs.m_mesh == rhs.m_mesh)
          return lhs.m_material < lhs.m_material;
        return lhs.m_mesh < rhs.m_mesh;
      }

      bool operator==(const MaterialModel& second) const;
  };
}
