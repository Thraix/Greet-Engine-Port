#include "Component.h"

#include <utils/AABBUtils.h>
#include <graphics/gui/GLayer.h>
#include <utils/UUID.h>

namespace Greet
{
  Component::Component(const XMLObject& object, Component* parent)
    : xmlObject(object.GetStrippedXMLObject()), parent(parent), m_isFocusable(false),isFocused(false)
  {
    size = Vec2(CalculateWidth(), CalculateHeight());
    if(xmlObject.HasProperty("name"))
      name = xmlObject.GetProperty("name");
    else
      name = "Component#" + std::to_string(UUID::GetInstance().GetUUID());

    normalStyle.Load("",*this);
    hoverStyle.Load("hover",*this,&normalStyle);
    pressStyle.Load("press",*this,&normalStyle);
    currentStyle = &normalStyle;
  }

  // Push translation to renderer
  void Component::PreRender(GUIRenderer* renderer, const Vec2& translation) const
  {
    renderer->PushMatrix(Mat3::Translate(translation));

    // Border around Component 
    if (xmlObject.HasProperty("borderColor"))
      //renderer->SubmitRect(pos + Vec2(0,0), size, currentStyle->borderColor, false);
     renderer->SubmitRoundedRect(pos+Vec2(0,0),size, currentStyle->borderColor, 10, 3, false);

    // Component background
    if (xmlObject.HasProperty("backgroundColor"))
      renderer->SubmitRoundedRect(pos + currentStyle->border.LeftTop(), size-GetBorder().LeftTop()-GetBorder().RightBottom(), currentStyle->backgroundColor, 10,3,false);
  }

  // Render component
  void Component::RenderHandle(GUIRenderer* renderer) const
  {
    // Render rest of component
    Render(renderer);
  }

  // Pop translation from renderer
  void Component::PostRender(GUIRenderer* renderer) const
  {
    renderer->PopMatrix();
  }

  void Component::UpdateHandle(float timeElapsed)
  {
    size = Vec2(GetWidth(), GetHeight());
    Update(timeElapsed);
  }

  Component* Component::OnMousePressed(const MousePressedEvent& event, const Vec2& translatedPos)
  {
    if(m_isFocusable)
    {
      MousePressed(event,translatedPos);
      return this;
    }
    return nullptr;

  }

  Component* Component::OnMouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos)
  {
    if(m_isFocusable)
    {
      MouseMoved(event,translatedPos);
      return this;
    }
    return nullptr;

  }
  void Component::OnFocused()
  {
    isFocused = true;
    Focused();
  }

  void Component::OnUnfocused()
  {
    isFocused = false;
    Unfocused();
  }

  Vec2 Component::GetPosition() const
  {
    return pos;
  }

  Component* Component::GetParent() const
  {
    return parent;
  }

  void Component::SetPosition(const Vec2& pos) 
  {
    this->pos = pos;
  }

  Vec2 Component::GetSize() const
  {
    return size;
  }

  void Component::SetSize(const Vec2& size)
  {
    this->size = size;
  }

  float Component::GetWidth() const 
  {
    return size.w;
  }

  float Component::GetHeight() const 
  {
    return size.h;
  }

  float Component::CalculateWidth() const
  {
    if (xmlObject.HasProperty("width"))
    {
      const std::string& w = xmlObject.GetProperty("width");
      if (parent == NULL)
        return GUIUtils::GetSize(w, GLayer::GetWidth());
      return GUIUtils::GetSize(w, parent->GetWidth() - parent->GetPadding().GetWidth() - parent->GetBorder().GetWidth());
    }
    return size.w;
  }

  float Component::CalculateHeight() const
  {
    if (xmlObject.HasProperty("height"))
    {
      const std::string& h = xmlObject.GetProperty("height");
      // If parent is nullptr it is the top component so use the window size
      if (parent == NULL)
        return GUIUtils::GetSize(h, GLayer::GetHeight());
      return GUIUtils::GetSize(h, parent->GetHeight() - parent->GetPadding().GetHeight() - parent->GetBorder().GetHeight());
    }
    return size.h;
  }

  Component* Component::GetComponentByName(const std::string& name)
  {
    if(name == this->name)
      return this;
    return nullptr;
  }

  void Component::ParentResized(const Vec2& parentSize)
  {
    Vec2 oldSize = size;
    size.w = CalculateWidth();
    size.h = CalculateHeight();
    if(oldSize != size)
      Resized();
  }

  const XMLObject& Component::GetXMLObject() const
  {
    return xmlObject;
  }

  bool Component::IsMouseInside(const Vec2& parentMouse) const
  {
    return AABBUtils::PointInsideBox(parentMouse, pos, size);
  }

  Vec2 Component::GetTotalPadding() const
  {
    return GetPadding().LeftTop() + GetBorder().LeftTop();
  }

  Vec2 Component::GetContentSize() const
  {
    return size - GetPadding().GetSize() - GetBorder().GetSize();
  }

  const TLBR& Component::GetMargin() const
  {
    return currentStyle->margin;
  }

  const TLBR& Component::GetPadding() const
  {
    return currentStyle->padding;
  }

  const TLBR& Component::GetBorder() const
  {
    return currentStyle->border;
  }
}
