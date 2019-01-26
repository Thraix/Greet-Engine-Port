#pragma once

#include <math/Maths.h>
#include <graphics/models/EntityModel.h>
#include <graphics/renderers/Renderer.h>
#include <internal/GreetGL.h>
#include <graphics/models/Camera.h>
#include <graphics/Skybox.h>
#include <graphics/layers/Scene.h>


namespace Greet{

  class Renderer3D : public Renderer
  {

    protected:

    public:
      Renderer3D()
      {}

      virtual ~Renderer3D() 
      {}

      virtual void Update(float timeElapsed) {}
      void BindMatrices(const Shader& shader, const Camera* camera, bool shouldBindShader = false) const;

      virtual void Begin(Camera* camera) {}
      virtual void Render(Camera* camera) const {}
      virtual void End(Camera* camera) {}
  };
}
