#pragma once

#include "GUIMouseListener.h"
#include "GUIUtils.h"
#include <graphics/gui/Component.h>
#include <graphics/renderers/GUIRenderer.h>
#include <event/MouseEvent.h>
#include <event/KeyEvent.h>
#include <graphics/gui/Style.h>

#include <vector>

namespace Greet {
  class Content : public Component
  {
    protected:
      bool m_isFocusable;
      bool isFocused;

      float ySpacing;

      Vec2 size;

    public:
      Content();
      Content(const XMLObject& object, Component* parent);
      virtual void Render(GUIRenderer* renderer, const Vec2& position) const{}
      virtual void Update(float timeElapsed){}

      // Returns the content that was focused
      Content* OnMousePressed(const MousePressedEvent& event, const Vec2& translatedPos) override;
      Content* OnMouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos) override;


      // These functions will only be called if the Content is focused
      virtual void MousePressed(const MousePressedEvent& event, const Vec2& translatedPos){}
      virtual void MouseReleased(const MouseReleasedEvent& event, const Vec2& translatedPos){}
      virtual void MouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos){}
      virtual void KeyPressed(const KeyPressedEvent& event){}
      virtual void KeyReleased(const KeyReleasedEvent& event){}

      virtual void OnFocused(){}
      virtual void OnUnfocused(){}
      virtual void MouseEntered(){}
      virtual void MouseExited(){}
      virtual bool IsFocusable() const { return m_isFocusable; };
  };
}
