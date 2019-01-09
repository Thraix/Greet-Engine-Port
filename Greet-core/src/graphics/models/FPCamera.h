#include "Camera.h"

namespace Greet {

  class FPCamera : public Camera
  {
    public:
      Vec3 position;
      float yaw;
      float pitch;
      float roll;

      const Vec3& GetRotationVector() const override { return Vec3(yaw,pitch,roll); }
      const Mat4& GetViewMatrix() const override { return Mat4::ViewMatrix(position, GetRotationVector()); }

  };
}
