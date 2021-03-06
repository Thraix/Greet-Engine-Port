#include "Layer3D.h"

namespace Greet {

  Layer3D::Layer3D(const Ref<Camera3D>& camera, const Ref<Skybox>& skybox)
    : camera{camera}, skybox{skybox}
  {
  }

  Layer3D::Layer3D(const Ref<Camera3D>& camera)
    : camera{camera}, skybox{nullptr}
  {
  }

  Layer3D::~Layer3D()
  {
  }

  void Layer3D::Render() const
  {
    if(skybox)
      skybox->Render(camera);
    for(auto&& renderer : renderers)
    {
      renderer->Begin(camera);
      renderer->Render(camera);
      renderer->End(camera);
    }
  }

  void Layer3D::Update(float timeElapsed)
  {
    camera->Update(timeElapsed);
    for(auto&& renderer : renderers)
    {
      renderer->Update(timeElapsed);
    }
  }

  void Layer3D::OnEvent(Event& event)
  {
    camera->OnEvent(event);
    Scene::OnEvent(event);
  }

  void Layer3D::ViewportResize(ViewportResizeEvent& event)
  {
    if(EVENT_IS_TYPE(event, EventType::VIEWPORT_RESIZE))
      camera->ViewportResize(event);
  }

  void Layer3D::AddRenderer(const Ref<Renderer3D>& renderer)
  {
    renderers.push_back(renderer);
  }

  void Layer3D::RemoveRenderer(const Ref<Renderer3D>& renderer)
  {
    for(auto it = renderers.begin(); it != renderers.end(); ++it)
    {
      if(*it == renderer)
      {
        renderers.erase(it);
        return;
      }
    }
    Log::Warning("Could not remove the renderer");
  }

  Vec3f Layer3D::GetWorldToScreenCoordinate(const Vec3f& coordinate) const
  {
    return camera->GetWorldToScreenCoordinate(coordinate);
  }

  Line Layer3D::GetScreenToWorldCoordinate(const Vec2f& screenPos) const
  {
    return camera->GetScreenToWorldCoordinate(screenPos);
  }
}
