#include "Slider.h"

#include <math/Maths.h>

namespace Greet
{
  uint Slider::SLIDER_FLAG_FORCE_INSIDE = BIT(0);
  uint Slider::SLIDER_FLAG_SNAP = BIT(1);

  Slider::Slider(const XMLObject& xmlObject, Component* parent)
    : Component(xmlObject, parent), flags(0), mouseHeld(false), stepSize(5)
  {
    m_isFocusable = true;
    if(xmlObject.GetObjectCount() > 0)
      sliderComponent = GUIUtils::GetComponent(xmlObject.GetObject(0), this);

    minValue = GUIUtils::GetFloatFromXML(xmlObject, "minValue", 0.0f);
    maxValue = GUIUtils::GetFloatFromXML(xmlObject, "maxValue", 100.0f);
    stepSize = GUIUtils::GetFloatFromXML(xmlObject, "stepSize", 0.0f);

    flags |= GUIUtils::GetBooleanFromXML(xmlObject, "indicatorInside", false);

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

    // Default defaultValue should be in the middle
    SetValue(GUIUtils::GetIntFromXML(xmlObject, "defaultValue", (maxValue-minValue)/2.0f));
  }

  void Slider::Render(GUIRenderer* renderer) const
  {
    sliderComponent->PreRender(renderer, pos + GetTotalPadding() + sliderComponent->GetMargin().LeftTop() + Vec2(sliderPos - sliderComponent->GetWidth()/2.0, (GetContentSize().h - sliderComponent->GetHeight())/2.0));
    sliderComponent->RenderHandle(renderer);
    sliderComponent->PostRender(renderer);
  }

  void Slider::MousePressed(const MousePressedEvent& event, const Vec2& translatedPos)
  {
    if(event.GetButton() == GLFW_MOUSE_BUTTON_1)
    {
      mouseHeld = true;
      SetValue(GetSliderValueFromPos(translatedPos.x));
    }
  }

  void Slider::MouseReleased(const MouseReleasedEvent& event, const Vec2& translatedPos)
  {
    if(event.GetButton() == GLFW_MOUSE_BUTTON_1)
    {
      mouseHeld = false;
    }
  }

  void Slider::MouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos)
  {
    if(mouseHeld)
    {
      SetValue(GetSliderValueFromPos(translatedPos.x));
    }
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
    return (pos / size.x) * (maxValue-minValue) + minValue;
  }

  float Slider::GetSliderPosFromValue(float value) const
  {
    if(flags & SLIDER_FLAG_FORCE_INSIDE)
    {
      return (value - minValue) / (maxValue - minValue) * (size.w-GetBorder().GetWidth()-sliderComponent->GetWidth())+sliderComponent->GetWidth()/2;
    }
    return (value - minValue) / (maxValue - minValue) * size.x;
  }
}
