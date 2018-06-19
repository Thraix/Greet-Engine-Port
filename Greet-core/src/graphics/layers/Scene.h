#pragma once

#include <event/InputControlListener.h>
#include <event/InputControl.h>

namespace Greet
{
  class Scene : public InputControlListener
  {
    public:
      virtual void PreRender() const {}
      virtual void Render() const = 0;
      virtual void PostRender() const {}
      virtual void PreUpdate(float timeElapsed) {}
      virtual void Update(float timeElapsed) = 0;
      virtual void PostUpdate(float timeElapsed) {}
      virtual InputControlRequest OnInputChanged(const InputControl* control) override { return InputControlRequest::NOTHING;}
  };
}
