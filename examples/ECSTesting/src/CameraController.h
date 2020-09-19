#pragma once

#include <ecs/NativeScript.h>
#include <ecs/ECSComponents.h>
#include <input/Input.h>
#include <event/KeyEvent.h>

class CameraController : public Greet::NativeScript
{
  private:
    Greet::Vec3f pos;
    Greet::Vec3f rot;

    Greet::Vec3f vel;
    Greet::Vec3f rotVel;
  public:
    CameraController(const Greet::Entity& entity, const Greet::Vec3f& pos = {}, const Greet::Vec3f& rot = {})
      : NativeScript{entity}, pos{pos}, rot{rot}, vel{}, rotVel{}
    {}

    void Update(float timeElapsed) override
    {
      ASSERT(entity.HasComponent<Greet::Camera3DComponent>(), "CameraController entity does not contain a Camera3DComponent");
      Greet::Camera3DComponent& cam = entity.GetComponent<Greet::Camera3DComponent>();
      pos += Greet::Vec::Rotate(vel, {0, 1, 0}, -rot.y) * timeElapsed;
      rot += rotVel * timeElapsed;
      cam.SetViewMatrix(Greet::Mat4::ViewMatrix(pos, rot));
    }

    void OnEvent(Greet::Event& event) override
    {
      float rotationSpeed = 4;
      float moveSpeed = 5;
      if(EVENT_IS_TYPE(event, Greet::EventType::KEY_PRESS))
      {
        Greet::KeyPressEvent& e = static_cast<Greet::KeyPressEvent&>(event);
        if(e.IsRepeat())
          return;
        switch(e.GetButton())
        {
          case GREET_KEY_DOWN:
            rotVel.x -= rotationSpeed; break;
          case GREET_KEY_UP:
            rotVel.x += rotationSpeed; break;
          case GREET_KEY_LEFT:
            rotVel.y += rotationSpeed; break;
          case GREET_KEY_RIGHT:
            rotVel.y -= rotationSpeed; break;
          case GREET_KEY_W:
            vel.z -= moveSpeed; break;
          case GREET_KEY_S:
            vel.z += moveSpeed; break;
          case GREET_KEY_A:
            vel.x -= moveSpeed; break;
          case GREET_KEY_D:
            vel.x += moveSpeed; break;
          case GREET_KEY_LEFT_SHIFT:
            vel.y -= moveSpeed; break;
          case GREET_KEY_SPACE:
            vel.y += moveSpeed; break;
        }
      }
      else if(EVENT_IS_TYPE(event, Greet::EventType::KEY_RELEASE))
      {
        Greet::KeyReleaseEvent& e = static_cast<Greet::KeyReleaseEvent&>(event);
        switch(e.GetButton())
        {
          case GREET_KEY_DOWN:
            rotVel.x += rotationSpeed; break;
          case GREET_KEY_UP:
            rotVel.x -= rotationSpeed; break;
          case GREET_KEY_LEFT:
            rotVel.y -= rotationSpeed; break;
          case GREET_KEY_RIGHT:
            rotVel.y += rotationSpeed; break;
          case GREET_KEY_W:
            vel.z += moveSpeed; break;
          case GREET_KEY_S:
            vel.z -= moveSpeed; break;
          case GREET_KEY_A:
            vel.x += moveSpeed; break;
          case GREET_KEY_D:
            vel.x -= moveSpeed; break;
          case GREET_KEY_LEFT_SHIFT:
            vel.y += moveSpeed; break;
          case GREET_KEY_SPACE:
            vel.y -= moveSpeed; break;
        }
      }
    }
};
