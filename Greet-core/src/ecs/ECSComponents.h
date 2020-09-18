#pragma once

#include <math/Maths.h>
#include <graphics/models/Material.h>
#include <graphics/models/Mesh.h>

namespace Greet
{
  struct Transform2DComponent
  {
    Mat3 transform;
  };

  struct Transform3DComponent
  {
    Mat4 transform;
  };

  struct TagComponent
  {
    std::string tag;
  };

  struct MeshComponent
  {
    Ref<Mesh> mesh;
  };

  struct MaterialComponent
  {
    Ref<Material> mesh;
  };
}

