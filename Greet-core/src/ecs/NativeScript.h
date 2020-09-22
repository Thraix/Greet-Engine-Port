#pragma once

#include <ecs/Entity.h>
#include <event/Event.h>

namespace Greet
{
  class NativeScript
  {
    protected:
      bool hasBoundEntity = false;
      Entity entity;
    public:
      NativeScript()
        : entity{nullptr}
      {}

      void BindEntity(const Entity& aEntity) { entity = aEntity; hasBoundEntity = true; }
      bool HasBoundEntity() const { return hasBoundEntity; }

      virtual void Create() {}
      virtual void Update(float timeElapsed) {}
      virtual void OnEvent(Event& event) {}
      virtual void Destroy() {}
  };
};
