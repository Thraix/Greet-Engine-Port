#include "Container.h"

#include "GUIUtils.h"
#include <utils/Utils.h>
#include <utils/AABBUtils.h>

namespace Greet {

  byte Container::RESIZING_LEFT = BIT(0);
  byte Container::RESIZING_RIGHT = BIT(1);
  byte Container::RESIZING_TOP = BIT(2);
  byte Container::RESIZING_BOTTOM = BIT(3);
  uint Container::RESIZING_MARGIN = 10;
  Container::Container()
    : Container(XMLObject())
  {
  
  }
  // These are usually the top element to no parent is needed
  Container::Container(const XMLObject& object)
    : Content(object, NULL)
  {
    m_resizableFlags = RESIZING_LEFT | RESIZING_RIGHT | RESIZING_TOP | RESIZING_BOTTOM;
    m_resizing = 0;
    minSize = Vec2(100, 100);

    // This is super ugly, need to find a better way to initialize these...
    if (object.HasProperty("minWidth"))
    {
      minSize.x = GUIUtils::CalcSize(object.GetProperty("minWidth"), Window::GetWidth());
    }
    if (object.HasProperty("minHeight"))
    {
      minSize.y = GUIUtils::CalcSize(object.GetProperty("minHeight"), Window::GetHeight());
    }
    if (object.HasProperty("resizeLeft"))
    {
      if (!GUIUtils::GetBoolean(object.GetProperty("resizeLeft")))
        m_resizableFlags &= ~RESIZING_LEFT;
    }
    if (object.HasProperty("resizeRight"))
    {
      if (!GUIUtils::GetBoolean(object.GetProperty("resizeRight")))
        m_resizableFlags &= ~RESIZING_RIGHT;
    }
    if (object.HasProperty("resizeTop"))
    {
      if (!GUIUtils::GetBoolean(object.GetProperty("resizeTop")))
        m_resizableFlags &= ~RESIZING_TOP;
    }
    if (object.HasProperty("resizeBottom"))
    {
      if (!GUIUtils::GetBoolean(object.GetProperty("resizeBottom")))
        m_resizableFlags &= ~RESIZING_BOTTOM;
    }
    pos = Vec2(0, 0);
    for (uint i = 0;i < object.GetObjectCount();i++)
    {
      AddContent(GUIUtils::GetContent(object.GetObject(i), this));
    }
  }

  Container::~Container()
  {
  }

  void Container::PreRender(GUIRenderer* renderer) const
  {
    renderer->PushViewport(pos, size, false);
  }

  void Container::PostRender(GUIRenderer* renderer) const
  {
    renderer->PopViewport();
  }

  bool Container::CheckResize(const Vec2& mousePos)
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

  void Container::Resize(const Vec2& mousePos)
  {
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
  }

  void Container::ResizeScreenClamp()
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

  void Container::SetGUIMouseListener(GUIMouseListener* listener)
  {
    m_mouseListener = listener;
  }

  void Container::OnWindowResize(int width, int height)
  {
    if (xmlObject.HasProperty("width"))
    {
      const std::string& w = xmlObject.GetProperty("width");
      if (!GUIUtils::IsStaticSize(w))
      {
        size.w = GUIUtils::CalcSize(w, width);
      }
    }
    if (xmlObject.HasProperty("height"))
    {
      const std::string& h = xmlObject.GetProperty("height");
      if (!GUIUtils::IsStaticSize(h))
      {
        size.h = GUIUtils::CalcSize(h, height);
      }
    }
  }

  void Container::OnMousePressed(const MousePressedEvent& event, const Vec2& translatedPos)
  {
    if (event.GetButton() == GLFW_MOUSE_BUTTON_1)
    {
      m_posOrigin = pos;
      m_sizeOrigin = size;
      m_clickPos = event.GetPosition();
      CheckResize(event.GetPosition());
    }
  }

  void Container::OnMouseReleased(const MouseReleasedEvent& event, const Vec2& translatedPos)
  {
    if (event.GetButton() == GLFW_MOUSE_BUTTON_1)
    {
      m_resizing = false;
    }
  }

  void Container::OnMouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos)
  {
    if (m_resizing)
    {
      Resize(event.GetPosition());
    }
  }

  bool Container::IsMouseInside(const Vec2& mousePos) const
  {
    Vec2 resizeMargin = Vec2(RESIZING_MARGIN, RESIZING_MARGIN);
    return AABBUtils::PointInsideBox(mousePos, -resizeMargin, GetSize() + resizeMargin*2);
  }


  void Container::OnFocused()
  {
    // Change the title to be more light, see other windows applications
  }

  void Container::OnUnfocused()
  {
    // Change the title to be more dark, see other windows applications
  }
}
