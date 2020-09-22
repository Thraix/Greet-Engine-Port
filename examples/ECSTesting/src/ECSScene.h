#pragma once

#include <graphics/layers/Scene.h>
#include <ecs/Entity.h>
#include <ecs/ECSManager.h>
#include <graphics/Skybox.h>
#include <graphics/renderers/BatchRenderer.h>

class ECSScene : public Greet::Scene
{
  private:
    Greet::Ref<Greet::ECSManager> manager;
    Greet::Ref<Greet::BatchRenderer> renderer2d;

  public:
    ECSScene();

    Greet::Entity AddEntity();
    void RemoveEntity(const Greet::Entity& entity);
    void RemoveEntity(EntityID entity);

    void Render() const override;
    void Render2D() const;
    void Render3D() const;
    void Update(float timeElapsed) override;
    void OnEvent(Greet::Event& event) override;
};

