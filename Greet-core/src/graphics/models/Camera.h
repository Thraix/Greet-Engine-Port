#pragma once

#include <math/Maths.h>
#include <event/Event.h>

namespace Greet {
  class Camera {

    private:
      Mat4 projectionMatrix;

    public:
      Camera(const Mat4& projectionMatrix) : projectionMatrix{projectionMatrix} {}
      virtual const Mat4& GetViewMatrix() const = 0;
      virtual const Mat4& GetProjectionMatrix() const { return projectionMatrix;}
      virtual void SetProjectionMatrix(const Mat4& projectionMatrix) 
      { 
        Camera::projectionMatrix = projectionMatrix;
      }
      virtual void Update(float timeElapsed) {};
      virtual void OnEvent(Event& event) = 0;
  };


}
