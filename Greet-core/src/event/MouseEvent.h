#pragma once

#include <event/Event.h>
#include <internal/GreetTypes.h>
#include <math/Maths.h>

namespace Greet {

  class MouseButtonEvent : public Event
  {
    protected:
      Vec2 position;
      uint button;
    protected:
      MouseButtonEvent(float x, float y, uint button)
        : Event(),position(x,y),button(button)
      {}
    public:
      inline const float GetX() const { return position.x; }
      inline const float GetY() const { return position.y; }
      inline const Vec2& GetPosition() const { return position; }
      inline const uint GetButton() const { return button; }

      uint GetCategory() const {return INPUT_EVENT | MOUSE_EVENT;}
  };

  class MousePressEvent : public MouseButtonEvent
  {
    public:
      MousePressEvent(float x, float y, uint button)
        : MouseButtonEvent(x, y, button) 
      {}

      EventType GetType() const {return EventType::MOUSE_PRESS;}
  };

  class MouseReleaseEvent : public MouseButtonEvent
  {
    public:
      MouseReleaseEvent(float x, float y, uint button)
        : MouseButtonEvent(x, y, button) 
      {}

      EventType GetType() const {return EventType::MOUSE_RELEASE;}
  };

  class MouseMoveEvent : public Event
  {
    private:
      Vec2 position;
      Vec2 deltaPosition;

    public:
      MouseMoveEvent(float x, float y, float dx, float dy)
        : position(Vec2(x,y)), deltaPosition(Vec2(dx,dy))
      {}

      inline const float GetX() const { return position.x; }
      inline const float GetY() const { return position.y; }
      inline const float GetDX() const { return deltaPosition.x; }
      inline const float GetDY() const { return deltaPosition.y; }
      inline const Vec2& GetPosition() const { return position; }
      inline const Vec2& GetDeltaPosition() const { return deltaPosition; }
      EventType GetType() const {return EventType::MOUSE_MOVE;}
      uint GetCategory() const {return INPUT_EVENT | MOUSE_EVENT;}
  };

  class MouseScrollEvent : public Event
  {
    private:
      float scrollVertical;
      float scrollHorizontal;
    public:
      MouseScrollEvent(float scrollVertical, float scrollHorizontal)
        : scrollVertical{scrollVertical}, scrollHorizontal{scrollHorizontal}
      {}

      inline const int GetScrollHorizontal() const { return scrollHorizontal; }
      inline const int GetScrollVertical() const { return scrollVertical; }
      uint GetCategory() const {return INPUT_EVENT | MOUSE_EVENT;}
      EventType GetType() const {return EventType::MOUSE_SCROLL;}

  };
}
