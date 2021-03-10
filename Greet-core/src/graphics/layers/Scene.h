#pragma once

#include <event/Event.h>
#include <event/ViewportEvent.h>

namespace Greet
{
  class Scene
  {
    public:
      virtual ~Scene() {}
      virtual void Render() const = 0;
      virtual void Update(float timeElapsed) = 0;
      virtual void OnEvent(Event& event)
      {
        if(EVENT_IS_TYPE(event, EventType::VIEWPORT_RESIZE))
        {
          ViewportResize((ViewportResizeEvent&) event);
        }
      }
      virtual void ViewportResize(ViewportResizeEvent& event) {}
  };
}
