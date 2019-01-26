#pragma once

#include <map>
#include <vector>
#include <graphics/models/EntityModel.h>
#include <graphics/models/Camera.h>
#include <utils/OBJUtils.h>
#include <graphics/textures/TextureManager.h>
#include <graphics/Skybox.h>
#include <graphics/models/MeshFactory.h>
#include <graphics/renderers/Renderer3D.h>

namespace Greet {

  class BatchRenderer3D : public Renderer3D
  {
    private:
      std::map<MaterialModel, std::vector<EntityModel*>> m_map;
    public:
      BatchRenderer3D()
        : Renderer3D(){}

      void Submit(EntityModel* model);
      void Render(Camera* camera) const override;
      void Update(float timeElapsed) override;
  };
}
