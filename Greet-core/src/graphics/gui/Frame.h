#pragma once

#include <vector>
#include "Container.h"
#include "GUIMouseListener.h"
#include <utils/xml/XML.h>

namespace Greet {
  class Frame : public Container 
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

    public:
      Frame();
      Frame(const XMLObject& object);
      virtual ~Frame();

      virtual bool IsMouseInside(const Vec2& mousePos) const override;

      // Check if the mouse is within the resize window and sets flags
      bool CheckResize(const Vec2& mousePos);

      // Resize the window the mouse position
      void Resize(const Vec2& mousePos);

      // Clamp the container inside window after resizing it.
      void ResizeScreenClamp();

      // Getters and setters
      virtual Vec2 GetContentPosition() const { return Vec2(currentStyle->margin.left + currentStyle->border.left, currentStyle->margin.top + currentStyle->border.top); };
      virtual Vec2 GetContentSize() const { return size - GetContentPosition() - Vec2(currentStyle->margin.right + currentStyle->border.right, currentStyle->margin.bottom + currentStyle->border.bottom); }

      // Set listeners
      void SetGUIMouseListener(GUIMouseListener* listener);

      // Listeners
      virtual void OnWindowResize(int width, int height);

      virtual Content* OnMousePressed(const MousePressedEvent& event, const Vec2& translatedPos) override;
      virtual void OnMouseReleased(const MouseReleasedEvent& event, const Vec2& translatedPos) override;
      virtual Content* OnMouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos) override;

      virtual void OnFocused();
      virtual void OnUnfocused();

  };
}
