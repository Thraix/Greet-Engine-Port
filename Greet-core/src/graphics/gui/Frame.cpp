#include "Frame.h"

#include "GUIUtils.h"
#include <utils/Utils.h>
#include <utils/AABBUtils.h>

namespace Greet {

  byte Frame::RESIZING_LEFT = BIT(0);
  byte Frame::RESIZING_RIGHT = BIT(1);
  byte Frame::RESIZING_TOP = BIT(2);
  byte Frame::RESIZING_BOTTOM = BIT(3);
  uint Frame::RESIZING_MARGIN = 10;

  Frame::Frame()
    : Frame(XMLObject())
  {

  }

  // These are usually the top element so no parent is needed
  Frame::Frame(const XMLObject& object)
    : Container(object, nullptr)
  {
    m_resizableFlags = RESIZING_LEFT | RESIZING_RIGHT | RESIZING_TOP | RESIZING_BOTTOM;
    m_resizing = 0;
    minSize = Vec2(100, 100);
    m_isFocusable = true;

    minSize.x = GUIUtils::GetSizeFromXML(object,"minWidth", 100, Window::GetWidth());
    minSize.y = GUIUtils::GetSizeFromXML(object,"minHeight", 100, Window::GetHeight());

    // This can still look better
    if(!GUIUtils::GetBooleanFromXML(object,"resizeLeft", false))
      m_resizableFlags &= ~RESIZING_LEFT;
    if(!GUIUtils::GetBooleanFromXML(object,"resizeRight", false))
      m_resizableFlags &= ~RESIZING_RIGHT;
    if(!GUIUtils::GetBooleanFromXML(object,"resizeTop", false))
      m_resizableFlags &= ~RESIZING_TOP;
    if(!GUIUtils::GetBooleanFromXML(object,"resizeBottom", false))
      m_resizableFlags &= ~RESIZING_BOTTOM;
  }

  Frame::~Frame()
  {
  }

  bool Frame::CheckResize(const Vec2& mousePos)
  {
    m_resizingFlags = 0;
    if ((m_resizableFlags & RESIZING_LEFT) != 0 && mousePos.x >= pos.x - RESIZING_MARGIN && mousePos.x < pos.x + RESIZING_MARGIN)
    {
      m_resizingFlags |= RESIZING_LEFT;
    }
    else if ((m_resizableFlags & RESIZING_RIGHT) != 0 && mousePos.x >= pos.x + size.x - RESIZING_MARGIN && mousePos.x < pos.x + size.x + RESIZING_MARGIN)
    {
      m_resizingFlags |= RESIZING_RIGHT;
    }
    if ((m_resizableFlags & RESIZING_TOP) != 0 && mousePos.y >= pos.y - RESIZING_MARGIN && mousePos.y < pos.y + RESIZING_MARGIN)
    {
      m_resizingFlags |= RESIZING_TOP;
    }
    else if ((m_resizableFlags & RESIZING_BOTTOM) != 0 && mousePos.y >= pos.y + size.y - RESIZING_MARGIN && mousePos.y < pos.y + size.y + RESIZING_MARGIN)
    {
      m_resizingFlags |= RESIZING_BOTTOM;
    }
    m_resizing = m_resizingFlags != 0;
    return m_resizing;
  }

  void Frame::Resize(const Vec2& mousePos)
  {
    Vec2 oldSize = size;
    Vec2 diff = m_posOrigin - (m_clickPos - mousePos);
    if (m_resizingFlags & RESIZING_LEFT)
    {
      pos.x = m_posOrigin.x - (m_clickPos.x - mousePos.x);
      size.x = m_sizeOrigin.x + (m_clickPos.x - mousePos.x);
      if (size.x < minSize.x)
      {
        pos.x = m_posOrigin.x + (m_sizeOrigin.x - minSize.x);
        size.x = minSize.x;
      }
    }
    else if (m_resizingFlags & RESIZING_RIGHT)
    {
      size.x = m_sizeOrigin.x - (m_clickPos.x - mousePos.x);
      if (size.x < minSize.x)
        size.x = minSize.x;
    }
    if (m_resizingFlags & RESIZING_TOP)
    {
      pos.y = m_posOrigin.y - (m_clickPos.y - mousePos.y);
      size.y = m_sizeOrigin.y + (m_clickPos.y - mousePos.y);
      if (size.y < minSize.y)
      {
        pos.y = m_posOrigin.y + (m_sizeOrigin.y - minSize.y);
        size.y = minSize.y;
      }
    }
    else if (m_resizingFlags & RESIZING_BOTTOM)
    {
      size.y = m_sizeOrigin.y - (m_clickPos.y - mousePos.y);
      if (size.y < minSize.y)
        size.y = minSize.y;
    }
    ResizeScreenClamp();
    if(oldSize != size)
      Resized();
  }

  void Frame::ResizeScreenClamp()
  {
    if (m_stayInsideWindow)
    {
      if (pos.x < 0)
      {
        pos.x = 0;
        size.x = m_posOrigin.x + m_sizeOrigin.x;
      }
      else if (pos.x > Window::GetWidth() - size.x)
      {
        size.x = Window::GetWidth() - m_posOrigin.x;
      }
      if (pos.y < 0)
      {
        pos.y = 0;
        size.y = m_posOrigin.y + m_sizeOrigin.y;
      }
      else if (pos.y > Window::GetHeight() - size.y)
        size.y = Window::GetWidth() - m_posOrigin.y;
    }
  }

  void Frame::SetGUIMouseListener(GUIMouseListener* listener)
  {
    m_mouseListener = listener;
  }

  void Frame::OnWindowResize(int width, int height)
  {
    if (xmlObject.HasProperty("width"))
    {
      const std::string& w = xmlObject.GetProperty("width");
      if (!GUIUtils::IsStaticSize(w))
      {
        size.w = GUIUtils::GetSize(w, width);
      }
    }
    if (xmlObject.HasProperty("height"))
    {
      const std::string& h = xmlObject.GetProperty("height");
      if (!GUIUtils::IsStaticSize(h))
      {
        size.h = GUIUtils::GetSize(h, height);
      }
    }
  }

  void Frame::MousePressed(const MousePressedEvent& event, const Vec2& translatedPos)
  {
    if (event.GetButton() == GLFW_MOUSE_BUTTON_1)
    {
      m_posOrigin = pos;
      m_sizeOrigin = size;
      m_clickPos = event.GetPosition();
      CheckResize(event.GetPosition());
    }
  }

  void Frame::MouseReleased(const MouseReleasedEvent& event, const Vec2& translatedPos)
  {
    if (event.GetButton() == GLFW_MOUSE_BUTTON_1)
    {
      m_resizing = false;
    }
  }

  void Frame::MouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos)
  {
    if (m_resizing)
    {

      Resize(event.GetPosition());
      // TODO: Recalculate children size if they are relative
    }
  }

  bool Frame::IsMouseInside(const Vec2& mousePos) const
  {
    Vec2 resizeMargin = Vec2(RESIZING_MARGIN, RESIZING_MARGIN);
    return AABBUtils::PointInsideBox(mousePos, -resizeMargin, GetSize() + resizeMargin*2);
  }


  void Frame::OnFocused()
  {
    // Change the title to be more light, see other windows applications
  }

  void Frame::OnUnfocused()
  {
    // Change the title to be more dark, see other windows applications
  }
}
