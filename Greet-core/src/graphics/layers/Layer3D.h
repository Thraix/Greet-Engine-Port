#pragma once

#include <graphics/renderers/Renderer3D.h>
#include <graphics/layers/Scene.h>

namespace Greet {
  class Layer3D : public Scene
  {
    protected:
      std::vector<Renderer3D*> renderers;
      Camera* camera;
      Skybox* skybox;

    public:
      Layer3D(Camera* camera, Skybox* skybox);
      virtual ~Layer3D();
      virtual void PreRender() const override;
      virtual void Render() const override;
      virtual void PostRender() const override;
      virtual void Update(float timeElapsed) override;
      virtual InputControlRequest OnInputChanged(const InputControl* control) override;

      void AddRenderer(Renderer3D* renderer);
      void RemoveRenderer(Renderer3D* renderer);

      Vec3 GetScreenCoordination(const Vec3& coordinate, uint screenWidth, uint screenHeight) const;
      void GetWorldCoordination(const Vec2& mousePos, Vec3* near, Vec3* direction) const;
  };
}
