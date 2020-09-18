#include "BatchRenderer3D.h"

namespace Greet {

  void BatchRenderer3D::Submit(EntityModel* model)
  {
    if(model == NULL)
    {
      Log::Error("Adding NULL EntityModel to 3D renderer");
      return;
    }

    auto it = m_map.find(model->GetMaterial());
    if(it == m_map.end())
    {
      std::vector<EntityModel*> vector{model};
      m_map.emplace(model->GetMaterial(), vector);
    }
    else
      (*it).second.push_back(model);
  }

  void BatchRenderer3D::Render(const Ref<Camera3D>& camera) const
  {
    for (auto&& entityModels : m_map)
    {
      entityModels.first->Bind();
      camera->SetShaderUniforms(entityModels.first->GetShader());
      for (auto&& entityModel : entityModels.second)
      {
        entityModel->GetMaterial()->GetShader()->SetUniformMat4("uTransformationMatrix", entityModel->GetTransformationMatrix());
        entityModel->PreRender();
        entityModel->Render();
        entityModel->PostRender();
      }
      entityModels.first->Unbind();
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
