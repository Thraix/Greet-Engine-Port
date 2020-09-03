#include "Frame.h"

#include <graphics/gui/GUIUtils.h>
#include <input/InputDefines.h>
#include <utils/Utils.h>
#include <utils/AABBUtils.h>

namespace Greet {

  REGISTER_FRAME_DEFINITION(Frame);

  byte Frame::RESIZING_LEFT = BIT(0);
  byte Frame::RESIZING_RIGHT = BIT(1);
  byte Frame::RESIZING_TOP = BIT(2);
  byte Frame::RESIZING_BOTTOM = BIT(3);
  uint Frame::RESIZING_MARGIN = 5;

  Frame::Frame()
    : Frame(XMLObject())
  {

  }

  // These are usually the top element so no parent is needed
  Frame::Frame(const XMLObject& object)
    : Container(object, nullptr), shouldCloseUnfocus{false}, m_stayInsideWindow{true}
  {
    if(object.HasAttribute("styling"))
    {
      std::string file = object.GetAttribute("styling");
      MetaFile metaFile{file};
      LoadFrameStyle(metaFile);
    }
    m_resizableFlags = RESIZING_LEFT | RESIZING_RIGHT | RESIZING_TOP | RESIZING_BOTTOM;
    m_resizing = 0;
    minSize = Vec2(100, 100);

    minSize.w = GUIUtils::GetFloatFromXML(object,"minWidth", 100);
    minSize.h = GUIUtils::GetFloatFromXML(object,"minHeight", 100);

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
    else if ((m_resizableFlags & RESIZING_RIGHT) != 0 && mousePos.x >= pos.x + width.size - RESIZING_MARGIN && mousePos.x < pos.x + width.size + RESIZING_MARGIN)
    {
      m_resizingFlags |= RESIZING_RIGHT;
    }
    if ((m_resizableFlags & RESIZING_TOP) != 0 && mousePos.y >= pos.y - RESIZING_MARGIN && mousePos.y < pos.y + RESIZING_MARGIN)
    {
      m_resizingFlags |= RESIZING_TOP;
    }
    else if ((m_resizableFlags & RESIZING_BOTTOM) != 0 && mousePos.y >= pos.y + height.size - RESIZING_MARGIN && mousePos.y < pos.y + height.size + RESIZING_MARGIN)
    {
      m_resizingFlags |= RESIZING_BOTTOM;
    }
    m_resizing = m_resizingFlags != 0;
    return m_resizing;
  }

  void Frame::Resize(const Vec2& mousePos)
  {
    Vec2 oldSize = GetSizeValue();
    Vec2 diff = m_posOrigin - (m_clickPos - mousePos);
    if (m_resizingFlags & RESIZING_LEFT)
    {
      pos.x = m_posOrigin.x - (m_clickPos.x - mousePos.x);
      width.value = m_sizeOrigin.x + (m_clickPos.x - mousePos.x);
      if (width.value < minSize.w)
      {
        pos.x = m_posOrigin.x + (m_sizeOrigin.x - minSize.w);
        width.value = minSize.w;
      }
    }
    else if (m_resizingFlags & RESIZING_RIGHT)
    {
      width.value = m_sizeOrigin.x - (m_clickPos.x - mousePos.x);
      if (width.value < minSize.w)
        width.value = minSize.w;
    }
    if (m_resizingFlags & RESIZING_TOP)
    {
      pos.y = m_posOrigin.y - (m_clickPos.y - mousePos.y);
      height.value = m_sizeOrigin.y + (m_clickPos.y - mousePos.y);
      if (height.value < minSize.h)
      {
        pos.y = m_posOrigin.y + (m_sizeOrigin.y - minSize.h);
        height.value = minSize.h;
      }
    }
    else if (m_resizingFlags & RESIZING_BOTTOM)
    {
      height.value = m_sizeOrigin.y - (m_clickPos.y - mousePos.y);
      if (height.value < minSize.h)
        height.value = minSize.h;
    }
    ResizeScreenClamp();
    if(oldSize != Vec2{width.value, height.value})
    {
      Remeasure();
    }
  }

  void Frame::ResizeScreenClamp()
  {
    if (m_stayInsideWindow)
    {
      if (pos.x < 0)
      {
        pos.x = 0;
        width.size = m_posOrigin.x + m_sizeOrigin.x;
      }
      else if (pos.x > guiScene->GetWidth() - width.size)
      {
        width.size = guiScene->GetWidth() - m_posOrigin.x;
      }
      if (pos.y < 0)
      {
        pos.y = 0;
        height.size = m_posOrigin.y + m_sizeOrigin.y;
      }
      else if (pos.y > guiScene->GetHeight() - height.size)
        height.size = guiScene->GetHeight() - m_posOrigin.y;
    }
  }

  void Frame::OnEvent(Event& event, const Vec2& componentPos)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      Vec2 translatedPos = e.GetPosition() - componentPos;
      if (e.GetButton() == GREET_MOUSE_1)
      {
        m_posOrigin = pos;
        m_sizeOrigin = {width.size, height.size};
        m_clickPos = translatedPos;
        CheckResize(translatedPos);
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
    {
      MouseReleaseEvent& e = static_cast<MouseReleaseEvent&>(event);
      if (e.GetButton() == GREET_MOUSE_1)
        m_resizing = false;
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
      Vec2 translatedPos = e.GetPosition() - componentPos;
      if (m_resizing)
        Resize(translatedPos);
    }
  }

  void Frame::SetGUIMouseListener(GUIMouseListener* listener)
  {
    m_mouseListener = listener;
  }

  void Frame::OnViewportResize(int width, int height)
  {
    Remeasure();
  }

  bool Frame::IsMouseInside(const Vec2& mousePos) const
  {
    Vec2 resizeMargin = Vec2(RESIZING_MARGIN, RESIZING_MARGIN);
    return AABBUtils::PointInsideBox(mousePos, -resizeMargin, GetSize() + resizeMargin*2);
  }

  void Frame::SetPosition(const Vec2& pos)
  {
    if(m_stayInsideWindow)
    {
      Vec2 p = pos;
      if(p.x + width.size > guiScene->GetWidth())
      {
        p.x = guiScene->GetWidth() - width.size;
      }
      if(p.y + height.size > guiScene->GetHeight())
      {
        p.y = guiScene->GetHeight() - height.size;
      }
      Component::SetPosition(p);
      return;
    }
    Component::SetPosition(pos);
  }

  void Frame::ChildChangedFocus(bool focused)
  {
    if(!GetParent() && !focused && shouldCloseUnfocus)
    {
      guiScene->RemoveFrameQueued(name);
    }
  }
}
