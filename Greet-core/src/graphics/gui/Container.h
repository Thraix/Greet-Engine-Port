#pragma once

#include <vector>
#include "Content.h"
#include "GUIMouseListener.h"
#include <utils/xml/XML.h>

namespace Greet {
  class Container : public Content
  {
    protected:

      static byte RESIZING_RIGHT;
      static byte RESIZING_LEFT;
      static byte RESIZING_TOP;
      static byte RESIZING_BOTTOM;
      static uint RESIZING_MARGIN;

      bool m_resizing;
      byte m_resizableFlags;
      byte m_resizingFlags;
      Vec2 minSize;

      bool m_stayInsideWindow;

      Vec2 m_clickPos;
      Vec2 m_posOrigin;
      Vec2 m_sizeOrigin;

      GUIMouseListener* m_mouseListener;

    public:
      bool visible;

      Vec2 pos;

    public:
      Container();
      Container(const XMLObject& object);
      virtual ~Container();
      virtual void PreRender(GUIRenderer* renderer) const;
      virtual void PostRender(GUIRenderer* renderer) const;

      virtual bool IsMouseInside(const Vec2& mousePos) const override;

      // Check if the mouse is within the resize window and sets flags
      bool CheckResize(const Vec2& mousePos);

      // Resize the window the mouse position
      void Resize(const Vec2& mousePos);

      // Clamp the container inside window after resizing it.
      void ResizeScreenClamp();

      // Getters and setters
      const Vec2& GetPosition() const {return pos;}
      virtual Vec2 GetContentPosition() const { return Vec2(margin.left + border.left, margin.top + border.top); };
      virtual Vec2 GetContentSize() const { return size - GetContentPosition() - Vec2(margin.right + border.right, margin.bottom + border.bottom); }

      // Set listeners
      void SetGUIMouseListener(GUIMouseListener* listener);

      // Listeners
      virtual void OnWindowResize(int width, int height);

      virtual void OnMousePressed(const MousePressedEvent& event, const Vec2& translatedPos) override;
      virtual void OnMouseReleased(const MouseReleasedEvent& event, const Vec2& translatedPos) override;
      virtual void OnMouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos) override;

      virtual void OnFocused();
      virtual void OnUnfocused();

  };
}
