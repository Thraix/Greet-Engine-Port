#pragma once

#include <event/Event.h>
#include <event/ViewportEvent.h>
#include <event/WindowEvent.h>
#include <graphics/RenderCommand.h>
#include <graphics/Window.h>
#include <logging/Log.h>
#include <math/Line.h>
#include <math/Maths.h>

namespace Greet {
  class Camera {

    protected:
      float fov;
      float near;
      float far;
      Mat4 projectionMatrix;
      Mat4 viewMatrix;

    public:
      Camera(float fov, float near, float far) :
        projectionMatrix{Mat4::PerspectiveViewport(fov, near, far)},
        fov{fov}, near{near}, far{far}
      {}

      virtual const Mat4& GetViewMatrix() const { return viewMatrix;}
      virtual const Mat4& GetProjectionMatrix() const { return projectionMatrix;}
      virtual void SetProjectionMatrix(const Mat4& projectionMatrix)
      {
        Camera::projectionMatrix = projectionMatrix;
      }
      virtual void Update(float timeElapsed) {};
      virtual void ViewportResize(ViewportResizeEvent& event)
      {
        projectionMatrix = Mat4::Perspective(event.GetWidth() / event.GetHeight(), fov, near, far);
      }
      virtual void OnEvent(Event& event) {};

      Vec3f GetWorldToScreenCoordinate(const Vec3f& coordinate) const
      {
        return projectionMatrix * (viewMatrix * coordinate);
      }

      Line GetScreenToWorldCoordinate(const Vec2f& screenPos) const
      {
        Line line;

        Mat4 pvInv = ~(projectionMatrix * viewMatrix);
        line.pos = pvInv * Vec3f(screenPos.x, screenPos.y, -1.0);
        Vec3f far = pvInv * Vec3f(screenPos.x, screenPos.y, 1.0);
        line.dir = far - line.pos;
        return line;
      }
  };
}
