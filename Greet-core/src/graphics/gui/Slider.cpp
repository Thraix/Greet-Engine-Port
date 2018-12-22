#include "Slider.h"

#include <math/Maths.h>

namespace Greet
{
  uint Slider::SLIDER_FLAG_FORCE_INSIDE = BIT(0);
  uint Slider::SLIDER_FLAG_SNAP = BIT(1);
  uint Slider::SLIDER_FLAG_VERTICAL = BIT(2);

  Slider::Slider(const XMLObject& xmlObject, Component* parent)
    : Component(xmlObject, parent), flags(0), stepSize(5)
  {
    m_isFocusable = true;
    if(xmlObject.GetObjectCount() > 0)
      sliderComponent = GUIUtils::GetComponent(xmlObject.GetObject(0), this);

    minValue = GUIUtils::GetFloatFromXML(xmlObject, "minValue", 0.0f);
    maxValue = GUIUtils::GetFloatFromXML(xmlObject, "maxValue", 100.0f);
    stepSize = GUIUtils::GetFloatFromXML(xmlObject, "stepSize", 0.0f);

    flags |= GUIUtils::GetBooleanFromXML(xmlObject, "indicatorInside", false) ? SLIDER_FLAG_FORCE_INSIDE : false;
    flags |= GUIUtils::GetBooleanFromXML(xmlObject, "vertical", false) ? SLIDER_FLAG_VERTICAL : false;

    if(maxValue - minValue < 0)
    {
      Log::Warning("Min value in slider is greater than max value");
      std::swap(minValue,maxValue);
    }

    //if((maxValue - minValue) / stepSize > 1.0001)
    //  Log::Warning("Step size in Slider doesn't divide max - min evenly");


    if(stepSize < 0)
    {
      stepSize = -stepSize;
      Log::Warning("Step size in Slider is negative");
    }

    if(stepSize > 0)
      flags |= SLIDER_FLAG_SNAP;

    // Resized initialized the min and max value of the positions
    Resized();

    // Default defaultValue should be in the middle
    SetValue(GUIUtils::GetIntFromXML(xmlObject, "defaultValue", (maxValue-minValue)/2.0f));
  }

  void Slider::Render(GUIRenderer* renderer) const
  {
    Vec2 sliderOffset = pos + GetTotalPadding() + sliderComponent->GetMargin().LeftTop() - sliderComponent->GetSize()/2;
    if(flags & SLIDER_FLAG_VERTICAL)
      sliderComponent->PreRender(renderer,  sliderOffset + Vec2{GetContentSize().w/2, sliderPos});
    else
      sliderComponent->PreRender(renderer,  sliderOffset + Vec2{sliderPos, GetContentSize().h/2});
    sliderComponent->RenderHandle(renderer);
    sliderComponent->PostRender(renderer);
  }

  void Slider::MousePressed(const MousePressedEvent& event, const Vec2& translatedPos)
  {
    Component::MousePressed(event,translatedPos);
    if(event.GetButton() == GLFW_MOUSE_BUTTON_1 && pressed)
    {
      if(flags & SLIDER_FLAG_VERTICAL)
        SetValue(GetSliderValueFromPos(translatedPos.y));
      else
        SetValue(GetSliderValueFromPos(translatedPos.x));
    }
  }

  void Slider::MouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos)
  {
    if(pressed)
    {
      if(flags & SLIDER_FLAG_VERTICAL)
        SetValue(GetSliderValueFromPos(translatedPos.y));
      else
        SetValue(GetSliderValueFromPos(translatedPos.x));
    }
  }

  void Slider::Resized() 
  { 
    float value = GetValue();
    // Is there a better way to do this. It is super sad.
    if(flags & SLIDER_FLAG_FORCE_INSIDE)
    {
      if(flags & SLIDER_FLAG_VERTICAL)
      {
        minPos = sliderComponent->GetHeight()/2;
        maxPos = size.h-GetBorder().GetHeight()-minPos;
      }
      else
      {
        minPos = sliderComponent->GetWidth()/2;
        maxPos = size.w-GetBorder().GetWidth()-minPos;
      }
    }
    else if(flags & SLIDER_FLAG_VERTICAL)
    {
      minPos = 0;
      maxPos = size.h;
    }
    else
    {
      minPos = 0;
      maxPos = size.w;
    }

    SetValue(value);
  }

  float Slider::GetValue() const
  {
    return GetSliderValueFromPos(sliderPos);
  }

  void Slider::SetValue(float value)
  {
    Math::Clamp(&value, minValue, maxValue);
    if(flags & SLIDER_FLAG_SNAP)
      value = GetSnappedSlider(value);
    sliderPos = GetSliderPosFromValue(value);
  }

  float Slider::GetSnappedSlider(float sliderValue) const
  {
    return Math::RoundClose(sliderValue,stepSize);
  }

  float Slider::GetSliderValueFromPos(float pos) const
  {
    return ((pos - minPos) / (maxPos - minPos)) * (maxValue-minValue) + minValue;
  }

  float Slider::GetSliderPosFromValue(float value) const
  {
    return (value - minValue) / (maxValue - minValue) * (maxPos-minPos) + minPos;
  }
}
