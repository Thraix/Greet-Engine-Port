#pragma once

#include <event/KeyEvent.h>

#include <logging/Log.h>

namespace Greet {

  class KeyListener
  {
    public:
      friend class EventDispatcher;
    private:
      uint m_id;
      static uint m_currentId;
    public:
      KeyListener() : m_id(m_currentId++) {}

      virtual void OnPressed(const KeyPressedEvent& e) {  ; }
      virtual void OnReleased(const KeyReleasedEvent& e) {  }
      virtual void OnTyped(const KeyTypedEvent& e) {  }
  };
}
