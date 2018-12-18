#include "Renderer3D.h"

namespace Greet{

  void Renderer3D::BindMatrices(const Shader& shader, bool shouldBindShader) const
  {
    if(shouldBindShader)
      shader.Enable();

    shader.SetUniformMat4("viewMatrix", m_camera->GetViewMatrix());
    shader.SetUniformMat4("projectionMatrix",GetProjectionMatrix());
  }

  void Renderer3D::RenderSkybox() const
  {
    if(m_skybox != NULL)
      m_skybox->Render(m_projectionMatrix, *m_camera);
  }

  void Renderer3D::Update(float timeElapsed)
  {
    m_camera->Update(timeElapsed);
  }

  Vec3 Renderer3D::GetScreenCoordination(const Vec3& coordinate, uint screenWidth, uint screenHeight) const
  {
    Vec4 point = (GetProjectionMatrix() * GetCamera()->GetViewMatrix()) * coordinate;
    Vec3 p = Vec3(point.x, point.y, point.z) / (fabs(point.z) * 2.0f) + 0.5f;
    p.x *= screenWidth;
    p.y = screenHeight - p.y * screenHeight;
    return p;
  }

  void Renderer3D::GetWorldCoordination(const Vec2& mousePos, Vec3* near, Vec3* direction) const
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
    Mat4 view = GetCamera()->GetViewMatrix();
    Mat4 proj = GetProjectionMatrix();
    Vec4 nearRes = ~view * ~proj * Vec3(mousePos.x, mousePos.y, -1.0);
    *near = Vec3(nearRes) / nearRes.w;
    Vec4 farRes = ~view * ~proj * Vec3(mousePos.x, mousePos.y, 1.0);
    *direction = ((Vec3(farRes) / farRes.w) - *near).Normalize();
  }
}
