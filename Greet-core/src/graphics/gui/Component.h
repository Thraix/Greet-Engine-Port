#pragma once

#include <graphics/gui/Style.h>
#include <graphics/renderers/GUIRenderer.h>
#include <event/MouseEvent.h>
#include <event/KeyEvent.h>

namespace Greet
{

  class Component
  {
    protected:
      Style* currentStyle;

      Style normalStyle;
      Style hoverStyle;
      Style pressStyle;

      XMLObject xmlObject;
      Component* parent;

      // Might get dirty?
      Vec2 pos;
      Vec2 size;

      bool m_isFocusable;
      bool isFocused;

    public:
      Component();
      Component(const XMLObject& object, Component* parent);
      // Push translation to renderer
      void PreRender(GUIRenderer* renderer, const Vec2& translation) const;

      // Render component
      virtual void RenderHandle(GUIRenderer* renderer) const;
      virtual void Render(GUIRenderer* renderer) const{}

      // Pop translation from renderer
      void PostRender(GUIRenderer* renderer) const;

      virtual void UpdateHandle(float timeElapsed);
      virtual void Update(float timeElapsed){}

      // Returns the focused content
      virtual Component* OnMousePressed(const MousePressedEvent& event, const Vec2& translatedPos);
      virtual void OnMouseReleased(const MouseReleasedEvent& event, const Vec2& translatedPos){}
      // Returns the hovered content
      virtual Component* OnMouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos);
      virtual void OnKeyPressed(const KeyPressedEvent& event){}
      virtual void OnKeyReleased(const KeyReleasedEvent& event){}

      Vec2 GetPosition() const;
      Vec2 GetSize() const;
      virtual float GetWidth() const;
      virtual float GetHeight() const;
      virtual float GetPotentialWidth() const;
      virtual float GetPotentialHeight() const;

      const XMLObject& GetXMLObject() const;
      virtual bool IsMouseInside(const Vec2& parentMouse) const;
      void SetPosition(const Vec2& pos);
      void SetSize(const Vec2& size);

      // These four will only be called if the component is focusable
      virtual void OnFocused(){}
      virtual void OnUnfocused(){}
      virtual void MouseEntered(){}
      virtual void MouseExited(){}


      // These functions will only be called if the component is focused
      virtual void MousePressed(const MousePressedEvent& event, const Vec2& translatedPos){}
      virtual void MouseReleased(const MouseReleasedEvent& event, const Vec2& translatedPos){}
      virtual void MouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos){}
      virtual void KeyPressed(const KeyPressedEvent& event){}
      virtual void KeyReleased(const KeyReleasedEvent& event){}


      Vec2 GetTotalPadding() const;
      const TLBR& GetMargin() const;
      const TLBR& GetPadding() const;
      const TLBR& GetBorder() const;
  };
}
