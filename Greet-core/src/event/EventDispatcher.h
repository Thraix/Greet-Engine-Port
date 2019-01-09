#pragma once

#include <map>
#include <vector>
#include <event/MouseEvent.h>
#include <event/MouseListener.h>
#include <event/KeyListener.h>

#define DISPATCHER_GUI BIT(24)

namespace Greet {
  class Window;
}

namespace Greet {
  class EventDispatcher
  {
    public:
      friend class Window;
    private:
      static std::map<uint, std::vector<MouseListener*>, std::greater<uint>> m_mouseListeners;
      static std::map<uint, std::vector<KeyListener*>, std::greater<uint>> m_keyListeners;

    public:
      static void AddMouseListener(uint priority, MouseListener& listener);
      static void AddKeyListener(uint priority, KeyListener& listener);
      static void RemoveMouseListener(MouseListener& listener);
      static void RemoveKeyListener(KeyListener& listener);

    private:
      static void OnMousePressed(const MousePressedEvent& e);
      static void OnMouseReleased(const MouseReleasedEvent& e);
      static void OnMouseMoved(const MouseMovedEvent& e);
      static void OnMouseScrolled(const MouseScrollEvent& e);

      static void OnKeyPressed(const KeyPressedEvent& e);
      static void OnKeyReleased(const KeyReleasedEvent& e);
      static void OnKeyTyped(const KeyTypedEvent& e);
  };
}
