#pragma once

#include <event/Event.h>
#include <common/Types.h>
#include <math/Maths.h>

namespace Greet {

  class MouseButtonEvent : public Event
  {
    protected:
      Vec2f position;
      uint button;
    protected:
      MouseButtonEvent(float x, float y, uint button)
        : Event(),position(x,y),button(button)
      {}
      MouseButtonEvent(const Vec2f& pos, uint button)
        : Event(),position(pos),button(button)
      {}
    public:
      float GetX() const { return position.x; }
      float GetY() const { return position.y; }
      const Vec2f& GetPosition() const { return position; }
      uint GetButton() const { return button; }

      uint GetCategory() const {return INPUT_EVENT | MOUSE_EVENT;}
  };

  class MousePressEvent : public MouseButtonEvent
  {
    public:
      MousePressEvent(float x, float y, uint button)
        : MouseButtonEvent(x, y, button)
      {}

      MousePressEvent(const Vec2f& pos, uint button)
        : MouseButtonEvent(pos, button)
      {}
      EventType GetType() const {return EventType::MOUSE_PRESS;}
  };

  class MouseReleaseEvent : public MouseButtonEvent
  {
    public:
      MouseReleaseEvent(float x, float y, uint button)
        : MouseButtonEvent(x, y, button)
      {}
      MouseReleaseEvent(const Vec2f& pos, uint button)
        : MouseButtonEvent(pos, button)
      {}

      EventType GetType() const {return EventType::MOUSE_RELEASE;}
  };

  class MouseMoveEvent : public Event
  {
    private:
      Vec2f position;
      Vec2f deltaPosition;

    public:
      MouseMoveEvent(float x, float y, float dx, float dy)
        : position(Vec2f(x,y)), deltaPosition(Vec2f(dx,dy))
      {}

      MouseMoveEvent(const Vec2f& pos, const Vec2f& delta)
        : position(pos), deltaPosition(delta)
      {}

      float GetX() const { return position.x; }
      float GetY() const { return position.y; }
      float GetDX() const { return deltaPosition.x; }
      float GetDY() const { return deltaPosition.y; }
      const Vec2f& GetPosition() const { return position; }
      const Vec2f& GetDeltaPosition() const { return deltaPosition; }
      EventType GetType() const {return EventType::MOUSE_MOVE;}
      uint GetCategory() const {return INPUT_EVENT | MOUSE_EVENT;}
  };

  class MouseScrollEvent : public Event
  {
    private:
      float scrollHorizontal;
      float scrollVertical;
    public:
      MouseScrollEvent(float scrollHorizontal, float scrollVertical)
        : scrollHorizontal{scrollHorizontal}, scrollVertical{scrollVertical}
      {}

      const float GetScrollHorizontal() const { return scrollHorizontal; }
      const float GetScrollVertical() const { return scrollVertical; }
      uint GetCategory() const {return INPUT_EVENT | MOUSE_EVENT;}
      EventType GetType() const {return EventType::MOUSE_SCROLL;}

  };
}
