#include "Camera.h"

namespace Greet {

  class FPCamera : public Camera
  {
    public:
      Vec3 position;
      float yaw;
      float pitch;
      float roll;
      Mat4 viewMatrix;

      FPCamera(const Mat4& projectionMatrix, const Vec3& position, const Vec3& rotation)
        : Camera(projectionMatrix), position(position), yaw(rotation.x), pitch(rotation.y), roll(rotation.z),
        viewMatrix(Mat4::ViewMatrix(position, Vec3(yaw,pitch, roll)))
      {
      
      }

      const Mat4& GetViewMatrix() const override { return viewMatrix; }

  };
}
