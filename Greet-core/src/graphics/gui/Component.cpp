#include "Component.h"

#include <utils/AABBUtils.h>
#include <graphics/gui/GLayer.h>
#include <utils/UUID.h>
#include <functional>
#include <cstdlib>

// TODO: REMOVE THIS INCLUDE
#include "TextBox.h"

namespace Greet
{


  Component::Component(const XMLObject& xmlObject, Component* parent)
    : xmlObject(xmlObject), parent(parent), m_isFocusable(false),isFocused(false), pos(0,0), pressed(false)
  {
    std::string width = xmlObject.GetProperty("width", "wrap_content");
    std::string height = xmlObject.GetProperty("height", "wrap_content");

    size.w = atof(width.c_str());
    size.h = atof(height.c_str());
    sizeType = size;

    if(width == "fill_parent")
    {
      widthSizeType = SizeType::WEIGHT;
      sizeType.w = 1;
    }
    else if(width[width.length()-1] == 'w')
      widthSizeType = SizeType::WEIGHT;
    else if(width == "wrap_content")
      widthSizeType = SizeType::WRAP;
    else
      widthSizeType = SizeType::NONE;


    if(height == "fill_parent")
    {
      heightSizeType = SizeType::WEIGHT;
      sizeType.h = 1;
    }
    else if(height[height.length()-1] == 'w')
      heightSizeType = SizeType::WEIGHT;
    else if(height == "wrap_content")
      heightSizeType = SizeType::WRAP;
    else
      heightSizeType = SizeType::NONE;

    if(xmlObject.HasProperty("name"))
      name = xmlObject.GetProperty("name");
    else
      name = xmlObject.GetName() + "#" + LogUtils::DecToHex(UUID::GetInstance().GetUUID(),8);

    normalStyle.Load("",*this);
    hoverStyle.Load("hover",*this,&normalStyle);
    pressStyle.Load("press",*this,&normalStyle);
    currentStyle = &normalStyle;
  }

  void Component::Measure()
  {
    Container* container = dynamic_cast<Container*>(parent);
    if(widthSizeType != SizeType::WEIGHT || (container && container->IsVertical() ))
    {
      if(widthSizeType == SizeType::WRAP)
        size.w = GetWrapSize().w;
      else
        size.h = sizeType.h;
    } 

    if(heightSizeType != SizeType::WEIGHT || (container && container->IsVertical() ))
    {
      if(heightSizeType == SizeType::WRAP)
        size.h = GetWrapSize().h;
      else
        size.h = sizeType.h;
    } 
  }

  void Component::MeasureFill()
  {
    MeasureFill(GLayer::GetWidth(), GLayer::GetHeight(), 1, true);
  }

  void Component::MeasureFill(float parentEmptyWidth, float parentEmptyHeight, float parentTotalWeight, bool vertical)
  {
    // Width
    if(widthSizeType == SizeType::WEIGHT)
    {
      if(vertical)
        size.w = parentEmptyWidth;
      else
        size.w = parentEmptyWidth * sizeType.w / parentTotalWeight;
    }
    // else we have already set the size in Measure


    // Height
    if(heightSizeType == SizeType::WEIGHT)
    {
      if(!vertical)
        size.h = parentEmptyHeight;
      else
        size.h = parentEmptyHeight * sizeType.h / parentTotalWeight;
    }
    // else we have already set the size in Measure

    if(dynamic_cast<TextBox*>(this))
    {
      Log::Info(size);
    }
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

  void Component::Remeasure()
  {
    if(!parent)
    {
      Measure();
      MeasureFill();
    }
    else
    {
      GetRootNode()->Remeasure();
    }
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

  Component::SizeType Component::GetWidthSizeType() const
  {
    return widthSizeType;
  }

  Component::SizeType Component::GetHeightSizeType() const
  {
    return heightSizeType;
  }

  const Vec2& Component::GetSizeType() const
  {
    return sizeType;
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

  Component* Component::GetRootNode()
  {
    Component* par = this;
    while(par->GetParent())
      par = par->GetParent();
    return par;
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
