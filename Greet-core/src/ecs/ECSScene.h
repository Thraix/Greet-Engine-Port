#pragma once

#include <graphics/layers/Scene.h>
#include <ecs/Entity.h>
#include <ecs/ECSManager.h>
#include <graphics/Skybox.h>
#include <graphics/renderers/BatchRenderer.h>

namespace Greet
{
  class ECSScene : public Greet::Scene
  {
    private:
      Ref<ECSManager> manager;
      Ref<BatchRenderer> renderer2d;

    public:
      ECSScene();

      Entity AddEntity();
      void RemoveEntity(const Entity& entity);
      void RemoveEntity(EntityID entity);

      void Render() const override;
      void Render2D() const;
      void Render3D() const;
      void Update(float timeElapsed) override;
      void OnEvent(Event& event) override;
  };
}
