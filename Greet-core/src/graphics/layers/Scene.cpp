#include "Scene.h"

namespace Greet
{
  Scene::Scene()
    : manager{new ECSManager{}}
  {}

  Entity Scene::AddEntity()
  {
    return Entity::Create(manager);
  }

  void Scene::PreRender()
  {
  }

  void Scene::Render() const
  {
  }

  void Scene::Scene::PostRender()
  {
  }

  void Scene::PreUpdate(float timeElapsed)
  {
  }

  void Scene::Update(float timeElapsed)
  {
  }

  void Scene::PostUpdate(float timeElapsed)
  {
  }

  void Scene::ViewportResize(ViewportResizeEvent& event)
  {
  }
}
