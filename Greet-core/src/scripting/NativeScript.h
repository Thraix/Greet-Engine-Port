#pragma once

#include <ecs/ECSScene.h>
#include <ecs/Entity.h>
#include <event/Event.h>

#define REGISTER_NATIVE_SCRIPT(SCRIPT_NAME)  extern "C" SCRIPT_NAME* New() { return new SCRIPT_NAME; }

namespace Greet
{
  struct NativeScript
  {
    Entity entity{nullptr};
    virtual ~NativeScript() {}

    virtual void OnCreate() {}
    virtual void OnUpdate(float timeElapsed) {}
    virtual void OnEvent(Event& event) {}
    virtual void OnDestroy() {}
  };
};
