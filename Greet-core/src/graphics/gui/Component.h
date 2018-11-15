#pragma once

#include <graphics/gui/Style.h>
#include <graphics/renderers/GUIRenderer.h>
#include <event/MouseEvent.h>
#include <event/KeyEvent.h>

namespace Greet
{
  class Content;

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

    public:
      Component();
      Component(const XMLObject& object, Component* parent);
      // Push translation to renderer
      void PreRender(GUIRenderer* renderer, const Vec2& translation) const;

      // Render component
      void RenderHandle(GUIRenderer* renderer) const;
      virtual void Render(GUIRenderer* renderer) const{}

      // Pop translation from renderer
      void PostRender(GUIRenderer* renderer) const;

      virtual void UpdateHandle(float timeElapsed);
      virtual void Update(float timeElapsed){}

      // Returns the focused content
      virtual Content* OnMousePressed(const MousePressedEvent& event, const Vec2& translatedPos){}
      virtual void OnMouseReleased(const MouseReleasedEvent& event, const Vec2& translatedPos){}
      // Returns the hovered content
      virtual Content* OnMouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos){}
      virtual void OnKeyPressed(const KeyPressedEvent& event){}
      virtual void OnKeyReleased(const KeyReleasedEvent& event){}

      Vec2 GetPosition() const;
      Vec2 GetSize() const;
      void SetSize(const Vec2& size);
      virtual float GetWidth() const;
      virtual float GetHeight() const;
      virtual float GetPotentialWidth() const;
      virtual float GetPotentialHeight() const;

      const XMLObject& GetXMLObject() const;

      virtual bool IsMouseInside(const Vec2& parentMouse) const;
  };
}
