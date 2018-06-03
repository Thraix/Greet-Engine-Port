#pragma once

#include <vector>
#include "Content.h"
#include "GUIMouseListener.h"
#include <utils/xml/XML.h>

namespace Greet {
  class Container
  {
    protected:

      static byte RESIZING_RIGHT;
      static byte RESIZING_LEFT;
      static byte RESIZING_TOP;
      static byte RESIZING_BOTTOM;
      static uint RESIZING_MARGIN;

      bool hasFocusedContent;

      bool m_resizing;
      byte m_resizableFlags;
      byte m_resizingFlags;
      Vec2 minSize;

      bool m_stayInsideWindow;

      Vec2 m_clickPos;
      Vec2 m_posOrigin;
      Vec2 m_sizeOrigin;
      XMLObject xmlObject;

      GUIMouseListener* m_mouseListener;

    public:
      float marginLeft;
      float marginRight;
      float marginTop;
      float marginBottom;

      float borderLeft;
      float borderRight;
      float borderTop;
      float borderBottom;

      Vec4 backgroundColor;
      Vec4 borderColor;

      bool visible;

      Vec2 pos;
      Vec2 size;
      Content* content;

    public:
      Container();
      Container(const XMLObject& object);
      Container(const Vec2& pos, const Vec2& size, Content* content);
      virtual ~Container();
      virtual void PreRender(GUIRenderer* renderer) const;
      virtual void Render(GUIRenderer* renderer) const;
      virtual void PostRender(GUIRenderer* renderer) const;
      virtual void Update(float timeElapsed);

      // Check if the mouse is within the resize window and sets flags
      bool CheckResize(const Vec2& mousePos);

      // Resize the window the mouse position
      void Resize(const Vec2& mousePos);

      // Clamp the container inside window after resizing it.
      void ResizeScreenClamp();

      // Getters and setters
      virtual Vec2 GetContentPosition() const { return Vec2(marginLeft + borderLeft, marginTop + borderTop); };
      virtual Vec2 GetContentSize() const { return size - GetContentPosition() - Vec2(marginRight + borderRight, marginBottom + borderBottom); }
      void SetMargins(float left, float right, float top, float bottom);

      // Set listeners
      void SetGUIMouseListener(GUIMouseListener* listener);

      // Listeners
      virtual void OnWindowResize(int width, int height);
      virtual bool OnPressed(const MousePressedEvent& event);
      virtual void OnReleased(const MouseReleasedEvent& event);
      virtual void OnMoved(const MouseMovedEvent& event);
      virtual void OnPressed(const KeyPressedEvent& event);
      virtual void OnReleased(const KeyReleasedEvent& event);
      virtual void OnFocused();
      virtual void OnUnfocused();

  };
}
