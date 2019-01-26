#include "Layer3D.h"

namespace Greet {

  Layer3D::Layer3D(Renderer3D* renderer, Camera* camera, Skybox* skybox)
    : m_renderer(renderer), camera{camera}, skybox{skybox}
  {
  }

  Layer3D::~Layer3D()
  {
    delete m_renderer;
  }

  void Layer3D::PreRender() const
  {
    m_renderer->Begin(camera);
    skybox->Render(camera->GetProjectionMatrix(), *camera);
  }

  void Layer3D::Render() const
  {
    m_renderer->Render(camera);
  }

  void Layer3D::PostRender() const 
  {
    m_renderer->End(camera);
  }

  void Layer3D::Update(float timeElapsed)
  {
    camera->Update(timeElapsed);
    m_renderer->Update(timeElapsed);
  }

  InputControlRequest Layer3D::OnInputChanged(const InputControl* control)
  {
    camera->OnInputChanged(control);
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
