#include "Renderer3D.h"

namespace Greet{

  void Renderer3D::BindMatrices(const Ref<Shader>& shader, const Ref<Camera3D>& camera, bool shouldBindShader) const
  {
    if(shouldBindShader)
      shader->Enable();
    camera->SetShaderUniforms(shader);
  }
}
