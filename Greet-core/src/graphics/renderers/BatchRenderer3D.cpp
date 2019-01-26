#include "BatchRenderer3D.h"

namespace Greet {

  void BatchRenderer3D::Submit(EntityModel* model)
  {
    if(model == NULL)
    {
      Log::Error("Adding NULL EntityModel to 3D renderer");
      return;
    }

    auto it = m_map.find(model->GetMaterialModel());
    if(it == m_map.end())
    {
      std::vector<EntityModel*> vector{model};
      m_map.emplace(model->GetMaterialModel(), vector);
    }
    else
      (*it).second.push_back(model);
  }

  void BatchRenderer3D::Render(Camera* camera) const
  {
    for (auto&& entityModels : m_map)
    {
      entityModels.first.GetMaterial().Bind();
      BindMatrices(entityModels.first.GetMaterial().GetShader(),camera);
      const Mesh& mesh = entityModels.first.GetMesh();
      mesh.Bind();
      for (auto&& entityModel : entityModels.second)
      {
        entityModels.first.GetMaterial().GetShader().SetUniformMat4("transformationMatrix", entityModel->GetTransformationMatrix());
        mesh.Render();
      }
        mesh.Unbind();
      entityModels.first.GetMaterial().Unbind();
    }
  }

  void BatchRenderer3D::Update(float timeElapsed) 
  {
    for (auto&& entityModels: m_map)
    {
      for (auto&& entityModel : entityModels.second)
      {
        entityModel->Update(timeElapsed);
      }
    }
  }
}
