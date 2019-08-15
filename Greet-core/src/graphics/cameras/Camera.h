#pragma once

#include <math/Maths.h>
#include <event/Event.h>

namespace Greet {
  class Camera {

    protected:
      Mat4 projectionMatrix;
      Mat4 viewMatrix;

    public:
      Camera(const Mat4& projectionMatrix) : projectionMatrix{projectionMatrix} {}
      virtual const Mat4& GetViewMatrix() const { return viewMatrix;}
      virtual const Mat4& GetProjectionMatrix() const { return projectionMatrix;}
      virtual void SetProjectionMatrix(const Mat4& projectionMatrix) 
      { 
        Camera::projectionMatrix = projectionMatrix;
      }
      virtual void Update(float timeElapsed) {};
      virtual void OnEvent(Event& event) = 0;

      Vec3<float> GetWorldToScreenCoordinate(const Vec3<float>& coordinate) const
      {
        Vec4 point = (projectionMatrix * viewMatrix) * coordinate;
        Vec3<float> p = Vec3<float>(point.x, point.y, point.z) / fabs(point.w);
        return p;
      }

      void GetScreenToWorldCoordinate(const Vec2& screenPos, Vec3<float>* near, Vec3<float>* direction) const
      {
        if (near == NULL)
          return Log::Error("Near vector is NULL");
        if (direction == NULL)
          return Log::Error("Direction vector is NULL");

        Mat4 pvInv = ~(projectionMatrix * viewMatrix);
        Vec4 nearRes = pvInv * Vec3<float>(screenPos.x, screenPos.y, -1.0);
        Vec4 farRes = pvInv * Vec3<float>(screenPos.x, screenPos.y, 1.0);

        *near = Vec3<float>(nearRes) / nearRes.w;
        *direction = ((Vec3<float>(farRes) / farRes.w) - * near).Normalize();
      }
  };


}
