#pragma once

#include <ecs/Entity.h>
#include <event/Event.h>

namespace Greet
{
  class NativeScript
  {
    protected:
      Entity entity;
    public:
      NativeScript(const Entity& entity)
        : entity{entity}
      {}

      virtual void Create() {}
      virtual void Update(float timeElapsed) {}
      virtual void OnEvent(Event& event) {}
      virtual void Destroy() {}
  };
};
