#pragma once

#include <graphics/models/Camera.h>
#include <math/Maths.h>
#include <event/MouseListener.h>
#include <internal/GreetGL.h>
#include <event/EventDispatcher.h>
#include <event/InputControlListener.h>

namespace Greet {

  class TPCamera : public Camera
  {
    private:

      std::string inputCameraRotate;
      std::string inputCameraZoom;
      std::string inputCameraXZ;
      std::string inputCameraY;
      std::string inputCameraMove;


      bool m_mouse1 = false;
      bool m_mouse2 = false;
      bool m_mouse3 = false;

      // Clamps to the camera position
      float m_distanceMin;
      float m_distanceMax;
      float m_heightMin;
      float m_heightMax;

      // Speed of the camera
      float m_rotationSpeed;
      float m_heightSpeed;
      float m_distanceSpeed;

      Vec3 m_position;
      float m_distance;
      float m_height;
      float m_rotation;

      Vec3 m_positionWanted;
      float m_distanceWanted;
      float m_heightWanted;
      float m_rotationWanted;

      // Calculated information
      Mat4 m_viewMatrix;
      Vec3 m_rotationVector;
    private:
      void CalculateRotationVector();
      void CalculateViewMatrix();
      void CalculateInformation();
    public:
      TPCamera();
      TPCamera(Vec3 position, float distance, float height, float rotation);
      TPCamera(Vec3 position, float distance, float height, float rotation, float distanceMin, float distanceMax, float heightMin, float heightMax);
      TPCamera(Vec3 position, float distance, float height, float rotation, float distanceMin, float distanceMax, float heightMin, float heightMax, float rotationSpeed, float heightSpeed, float distanceSpeed);
      virtual ~TPCamera();

      const Vec3& GetRotationVector() const override;
      const Mat4& GetViewMatrix() const override;
      void Update(float timeElapsed) override;

      const Vec3& GetPosition() const { return m_position; }
      float GetHeight() const { return m_height; }
      float GetRotation() const { return m_rotation; }
      float GetDistance() const { return m_distance; }
      float GetMinDistance() const { return m_distanceMin; }
      float GetMaxDistance() const { return m_distanceMax; }
      float GetMinHeight() const { return m_heightMin; }
      float GetMaxHeight() const { return m_heightMax; }


      void SetPosition(Vec3 pos);
      void SetHeight(float height);
      void SetRotation(float rotation);
      void SetDistance(float distance);
      void SetDistanceClamp(float min, float max);
      void SetHeightClamp(float min, float max);

      void SetInputRotation(const std::string& inputName);
      void SetInputZoom(const std::string& inputName);
      void SetInputXZ(const std::string& inputName);
      void SetInputY(const std::string& inputName);
      void SetInputMove(const std::string& inputName);

      void Move(const Vec2& delta);
      void Zoom(float delta);
      InputControlRequest OnInputChanged(const InputControl* control) override;

  };

}
