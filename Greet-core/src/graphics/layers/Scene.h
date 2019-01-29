#pragma once

#include <event/Event.h>
#include <event/WindowEvent.h>

namespace Greet
{
  class Scene
  {
    public:
      virtual void PreRender() const {}
      virtual void Render() const = 0;
      virtual void PostRender() const {}
      virtual void PreUpdate(float timeElapsed) {}
      virtual void Update(float timeElapsed) = 0;
      virtual void PostUpdate(float timeElapsed) {}
      virtual void OnEvent(Event& event)
      {
        if(EVENT_IS_TYPE(event, EventType::WINDOW_RESIZE))
        {
          WindowResize((WindowResizeEvent&) event);
        }
      }
      virtual void WindowResize(WindowResizeEvent& event) {}
  };
}
