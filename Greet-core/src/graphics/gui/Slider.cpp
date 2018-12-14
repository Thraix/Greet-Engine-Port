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

    minValue = GUIUtils::GetIntFromXML(xmlObject, "minValue", 0);
    maxValue = GUIUtils::GetIntFromXML(xmlObject, "maxValue", 100);

    // Default defaultValue should be in the middle
    float sliderValue = GUIUtils::GetIntFromXML(xmlObject, "defaultValue", (maxValue-minValue)/2.0f);

    sliderPos = GetSnappedSlider(GetSliderPosFromValue(sliderValue));
  }

  void Slider::Render(GUIRenderer* renderer) const
  {
    sliderComponent->PreRender(renderer, pos + GetTotalPadding() + sliderComponent->GetMargin().LeftTop() + Vec2(sliderPos - sliderComponent->GetWidth()/2.0 - sliderComponent->GetBorder().left, (size.h - sliderComponent->GetHeight())/2.0));
    sliderComponent->RenderHandle(renderer);
    sliderComponent->PostRender(renderer);
  }

  void Slider::MousePressed(const MousePressedEvent& event, const Vec2& translatedPos)
  {
    if(event.GetButton() == GLFW_MOUSE_BUTTON_1)
    {
      mouseHeld = true;
      sliderPos = translatedPos.x;
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
      sliderPos = GetSliderPosFromValue(GetSnappedSlider(GetSliderValueFromPos(translatedPos.x)));
      Math::Clamp(&sliderPos, 0.0f, size.x);
    }
  }

  float Slider::GetSnappedSlider(float sliderValue)
  {
    return Math::RoundClose(sliderValue,stepSize);
  }

  float Slider::GetSliderValueFromPos(float pos)
  {
    return (pos / size.x) * (maxValue-minValue) + minValue;
  }

  float Slider::GetSliderPosFromValue(float value)
  {
    return (value - minValue) / (maxValue - minValue) * size.x;
  }
}
