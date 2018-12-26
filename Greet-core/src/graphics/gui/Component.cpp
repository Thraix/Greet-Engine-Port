#include "Component.h"

#include <utils/AABBUtils.h>
#include <graphics/gui/GLayer.h>
#include <utils/UUID.h>
#include <functional>
#include <cstdlib>

namespace Greet
{
  Component::Component(const XMLObject& object, Component* parent)
    : xmlObject(object.GetStrippedXMLObject()), parent(parent), m_isFocusable(false),isFocused(false), pos(0,0), pressed(false)
  {
    size = Vec2(100,10);
    if(xmlObject.HasProperty("name"))
      name = xmlObject.GetProperty("name");
    else
      name = "Component#" + LogUtils::DecToHex(UUID::GetInstance().GetUUID(),8);

    normalStyle.Load("",*this);
    hoverStyle.Load("hover",*this,&normalStyle);
    pressStyle.Load("press",*this,&normalStyle);
    currentStyle = &normalStyle;
  }

  void Component::Measure()
  {
    float weight = atof(xmlObject.GetProperty("weigth", "0").c_str());
    std::string width = xmlObject.GetProperty("width", "wrap_content");
    std::string height = xmlObject.GetProperty("height", "wrap_content");
    Container* container = dynamic_cast<Container*>(parent);
    if(weight == 0.0f || (container && container->IsVertical() ))
    {
      if(width == "wrap_content")
      {
        size.w = GetWrapSize().w;
      }
      else
      {
        float w = atof(width.c_str());
        if(w > 0)
        {
          size.w = w;
        }
        else if(width == "fill_parent")
        {
          // If we have a parent, let that parent determain the size
          if(!parent)
          {
            GLayer::GetWidth();
          }
        }
      }
    } 
    else if(!parent)
    {
      GLayer::GetWidth();
    }// else let the parent determain the size

    if(weight == 0.0f || (container && !container->IsVertical() ))
    {
      if(height == "wrap_content")
      {
        size.h = GetWrapSize().h;
      }
      else
      {
        float h = atof(height.c_str());
        if(h > 0)
        {
          size.h = h;
        }
        else if(height == "fill_parent")
        {
          // If we have a parent, let that parent determain the size
          if(!parent)
          {
            GLayer::GetHeight();
          }
        }
      }
    } 
    else if(!parent)
    {
      GLayer::GetHeight();
    }// else let the parent determain the size
  }

  void Component::MeasureFill(float parentEmptyWidth, float parentEmptyHeight, float parentTotalWeight, bool vertical)
  {
    float weight = atof(xmlObject.GetProperty("weight", "0").c_str());
    std::string width = xmlObject.GetProperty("width", "wrap_content");
    std::string height = xmlObject.GetProperty("height", "wrap_content");

    // Width
    if(vertical)
    {
      if(width == "fill_parent")
        size.w = parentEmptyWidth;
      // else we have already set the size in Measure
    }
    else if(weight > 0 && !vertical) 
    {
      size.w = parentEmptyWidth * weight / parentTotalWeight;
    }
    else if(width == "fill_parent")
    {
      size.h = parentEmptyHeight / parentTotalWeight;
    }
    // else we have already set the size in Measure


    // Height
    if(!vertical)
    {
      if(height == "fill_parent")
        size.h = parentEmptyHeight;
      // else we have already set the size in Measure
    }
    else if(weight > 0 && vertical )
    {
      size.h = parentEmptyHeight * weight / parentTotalWeight;
    }
    else if(height == "fill_parent")
    {
      size.h = parentEmptyHeight / parentTotalWeight;
    }
    // else we have already set the size in Measure
    //
    OnMeasured();
  }

  Vec2 Component::GetMeasureFillSize()
  {
    return GetContentSize();
  }

  float Component::GetMeasureTotalWeight()
  {
    return 1;
  }

  // Push translation to renderer
  void Component::PreRender(GUIRenderer* renderer, const Vec2& translation) const
  {
    renderer->PushMatrix(Mat3::Translate(translation));

    // Border around Component 
    if (xmlObject.HasProperty("borderColor"))
      //renderer->SubmitRect(pos + Vec2(0,0), size, currentStyle->borderColor, false);
      renderer->SubmitRoundedRect(pos+Vec2(0,0),size, currentStyle->borderColor, currentStyle->borderRadius, currentStyle->roundedPrecision, false);

    // Component background
    if (xmlObject.HasProperty("backgroundColor"))
      renderer->SubmitRoundedRect(pos + currentStyle->border.LeftTop(), size-GetBorder().LeftTop()-GetBorder().RightBottom(), currentStyle->backgroundColor, currentStyle->radius,currentStyle->roundedPrecision,false);
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
  void Component::SetOnClickCallback(OnClickCallback callback)
  {
    onClickCallback = callback;
  }

  void Component::SetOnPressCallback(OnPressCallback callback)
  {
    onPressCallback = callback;
  }

  void Component::SetOnReleaseCallback(OnReleaseCallback callback)
  {
    onReleaseCallback = callback;
  }

  void Component::CallOnClickCallback()
  {
    if(onClickCallback)
      onClickCallback(this);
  }

  void Component::CallOnPressCallback()
  {
    if(onPressCallback)
      onPressCallback(this);
  }

  void Component::CallOnReleaseCallback()
  {
    if(onReleaseCallback)
      onReleaseCallback(this);
  }

  void Component::MousePressed(const MousePressedEvent& event, const Vec2& translatedPos)
  {
    if(event.GetButton() == GLFW_MOUSE_BUTTON_1)
    {
      pressed = true;
      CallOnPressCallback();
    }
  }

  void Component::MouseReleased(const MouseReleasedEvent& event, const Vec2& translatedPos)
  {
    if(event.GetButton() == GLFW_MOUSE_BUTTON_1)
    {
      if(pressed)
      {
        if(IsMouseInside(translatedPos+pos))
          CallOnClickCallback();
        CallOnReleaseCallback();
      }
      pressed = false;
    }
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

  Vec2 Component::GetRealPosition() const
  {
    return pos + GetMargin().LeftTop() + (parent ? parent->GetTotalPadding()+parent->GetRealPosition() : Vec2(0,0));
  }

  Vec2 Component::GetWrapSize() const
  {
    return Vec2(100,100);
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

  Component* Component::GetComponentByNameNoCast(const std::string& name)
  {
    if(name == this->name)
      return this;
    return nullptr;
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

  const std::string& Component::GetName() const
  {
    return name;
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
