#include "Content.h"

#include <graphics/gui/GUIUtils.h>
#include <utils/AABBUtils.h>

namespace Greet {

  Content::Content()
    : Component(XMLObject(),nullptr), m_isFocusable(true)
  {
  }

  Content::Content(const XMLObject& object, Component* parent)
    : Component(object,parent), m_isFocusable(true)
  {
  }

  Content* Content::OnMousePressed(const MousePressedEvent& event, const Vec2& translatedPos)
  {
    if(m_isFocusable)
    {
      MousePressed(event,translatedPos);
      return this;
    }
    return nullptr;
  }

  Content* Content::OnMouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos)
  {
    if(m_isFocusable)
    {
      MouseMoved(event,translatedPos);
      return this;
    }
    return nullptr;
  
  }
}
