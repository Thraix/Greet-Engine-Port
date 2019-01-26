#pragma once

#include <math/Maths.h>
#include <event/InputControlListener.h>
//#include <graphics/models/EntityModel.h>
//#include <graphics/renderers/Renderer3D.h>

namespace Greet {
  class Camera {

    private:
      Mat4 projectionMatrix;

    public:
      Camera(const Mat4& projectionMatrix) : projectionMatrix{projectionMatrix} {}
      virtual const Vec3& GetRotationVector() const = 0;
      virtual const Mat4& GetViewMatrix() const = 0;
      virtual const Mat4& GetProjectionMatrix() const { return projectionMatrix;}
      virtual void SetProjectionMatrix(const Mat4& projectionMatrix) 
      { 
        Camera::projectionMatrix = projectionMatrix;
      }
      virtual void Update(float timeElapsed) {};
      virtual InputControlRequest OnInputChanged(const InputControl* control) = 0;
  };


}
