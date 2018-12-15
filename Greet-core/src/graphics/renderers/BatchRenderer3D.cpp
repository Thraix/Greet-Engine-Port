#include "BatchRenderer3D.h"

namespace Greet {

  void BatchRenderer3D::Submit(const EntityModel* model)
  {
    if(model == NULL)
    {
      Log::Error("Adding NULL EntityModel to 3D renderer");
      return;
    }
    for (BatchRenderer3DMap* map : m_map)
    {
      if (map->m_material == model->GetMaterialModel())
      {
        map->AddEntity(model);
        return;
      }
    }
    BatchRenderer3DMap* map = new BatchRenderer3DMap(model->GetMaterialModel());
    map->AddEntity(model);
    m_map.push_back(map);
  }

  void BatchRenderer3D::Render() const
  {
    GLCall(glDepthRange(m_near, m_far));
    for (BatchRenderer3DMap* map : m_map)
    {
      map->m_material.GetMaterial().Bind();
      BindMatrices(map->m_material.GetMaterial().GetShader());
      const Mesh& mesh = map->m_material.GetMesh();
      mesh.Bind();
      for (const EntityModel* model : map->m_models)
      {
        map->m_material.GetMaterial().GetShader().SetUniformMat4("transformationMatrix", model->GetTransformationMatrix());
        mesh.Render();
      }
      mesh.Unbind();
      map->m_material.GetMaterial().Unbind();
    }

  }
}
