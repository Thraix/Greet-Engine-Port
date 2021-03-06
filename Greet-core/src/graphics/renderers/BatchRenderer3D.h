#pragma once

#include <map>
#include <vector>
#include <graphics/models/EntityModel.h>
#include <utils/OBJUtils.h>
#include <utils/Utils.h>
#include <graphics/textures/TextureManager.h>
#include <graphics/Skybox.h>
#include <graphics/models/MeshFactory.h>
#include <graphics/renderers/Renderer3D.h>

namespace Greet {

  class BatchRenderer3D : public Renderer3D
  {
    private:
      std::map<Ref<Material>, std::vector<Ref<EntityModel>>, RefLess<Material>> m_map;
    public:
      BatchRenderer3D()
        : Renderer3D(){}

      void Submit(const Ref<EntityModel>& model);
      void Render(const Ref<Camera3D>& camera) const override;
      void Update(float timeElapsed) override;
  };
}
