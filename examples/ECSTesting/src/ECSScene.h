#pragma once

#include <graphics/layers/Scene.h>
#include <ecs/Entity.h>
#include <ecs/ECSManager.h>
#include <graphics/Skybox.h>

class ECSScene : public Greet::Scene
{
  private:
    Greet::Ref<Greet::ECSManager> manager;
    Greet::Ref<Greet::Skybox> skybox;

  public:
    ECSScene();

    Greet::Entity AddEntity();
    void RemoveEntity(const Greet::Entity& entity);
    void RemoveEntity(EntityID entity);

    void Render() const override;
    void Update(float timeElapsed) override;
    void OnEvent(Greet::Event& event) override;
};

