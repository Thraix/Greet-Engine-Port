#include "Layer3D.h"

namespace Greet {

  Layer3D::Layer3D(Camera* camera, Skybox* skybox)
    : camera{camera}, skybox{skybox}
  {
  }

  Layer3D::Layer3D(Camera* camera)
    :camera{camera}, skybox{nullptr}
  {
  
  }

  Layer3D::~Layer3D()
  {
  }

  void Layer3D::PreRender() const
  {
    if(skybox)
      skybox->Render(*camera);
  }

  void Layer3D::Render() const
  {
    for(auto&& renderer : renderers)
    {
      renderer->Begin(camera);
      renderer->Render(camera);
      renderer->End(camera);
    }
  }

  void Layer3D::PostRender() const 
  {
  }

  void Layer3D::Update(float timeElapsed)
  {
    camera->Update(timeElapsed);
    for(auto&& renderer : renderers)
    {
      renderer->Update(timeElapsed);
    }
  }

  InputControlRequest Layer3D::OnInputChanged(const InputControl* control)
  {
    camera->OnInputChanged(control);
    return InputControlRequest::NOTHING;
  }

  void Layer3D::AddRenderer(Renderer3D* renderer)
  {
    renderers.push_back(renderer);
  }

  void Layer3D::RemoveRenderer(Renderer3D* renderer)
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

  Vec3 Layer3D::GetScreenCoordination(const Vec3& coordinate, uint screenWidth, uint screenHeight) const
  {
    Vec4 point = (camera->GetProjectionMatrix() * camera->GetViewMatrix()) * coordinate;
    Vec3 p = Vec3(point.x, point.y, point.z) / (fabs(point.z) * 2.0f) + 0.5f;
    p.x *= screenWidth;
    p.y = screenHeight - p.y * screenHeight;
    return p;
  }

  void Layer3D::GetWorldCoordination(const Vec2& mousePos, Vec3* near, Vec3* direction) const
  {
    if (near == NULL)
    {
      Log::Error("Near vector is NULL");
      return;
    }
    if (direction == NULL)
    {
      Log::Error("Direction vector is NULL");
      return;
    }
    Mat4 view = camera->GetViewMatrix();
    Vec4 nearRes = ~view * ~camera->GetProjectionMatrix() * Vec3(mousePos.x, mousePos.y, -1.0);
    *near = Vec3(nearRes) / nearRes.w;
    Vec4 farRes = ~view * ~camera->GetProjectionMatrix() * Vec3(mousePos.x, mousePos.y, 1.0);
    *direction = ((Vec3(farRes) / farRes.w) - *near).Normalize();
  }
}
