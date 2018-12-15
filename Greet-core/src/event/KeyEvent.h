#pragma once

#include <math/Maths.h>
#include <event/Event.h>

namespace Greet {

  class KeyButtonEvent : public Event
  {
    protected:
      uint m_button;
    protected:
      KeyButtonEvent(uint button, EventType type)
        : Event(type), m_button(button) {}
    public:
      inline const uint GetButton() const { return m_button; }
  };

  class KeyPressedEvent : public KeyButtonEvent
  {
    public:
      KeyPressedEvent(uint button)
        : KeyButtonEvent(button, KEY_PRESSED) {}
  };

  class KeyReleasedEvent : public KeyButtonEvent
  {
    public:
      KeyReleasedEvent(uint button)
        : KeyButtonEvent(button, KEY_RELEASED) {}
  };

  class KeyTypedEvent : public Event
  {
    private:
      uint m_charCode;

    public:
      KeyTypedEvent(uint charCode) : Event(KEY_TYPED), m_charCode(charCode) {}
      uint GetCharCode() const { return m_charCode; }
  };

}
