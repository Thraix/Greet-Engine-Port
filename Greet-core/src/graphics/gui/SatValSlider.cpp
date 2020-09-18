#include "SatValSlider.h"

#include <input/InputDefines.h>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(SatValSlider);

  SatValSlider::SatValSlider(const std::string& name, Component* parent, const std::string& componentType)
    : Component{name, parent, componentType}, color{0, 0.5, 0.5, 1}
  {}

  SatValSlider::SatValSlider(const XMLObject& xmlObject, Component* parent)
    : Component(xmlObject, parent), color{0, 0.5, 0.5, 1}
  {}


  void SatValSlider::PreRender(GUIRenderer* renderer, const Vec2f& translation) const
  {
    renderer->PushTranslation(translation);
    renderer->DrawRect(pos, GetSize(), Color(color.h, 0, 0), Color(color.h, 1, 0), Color(color.h, 0, 1), Color(color.h, 1, 1), true);
  }

  void SatValSlider::Render(GUIRenderer* renderer) const
  {
    Vec2f sliderPos = {GetSliderPosFromSat(color.s), GetSliderPosFromVal(color.v)};
    Vec2f size = GetSize();
    Color argb = Color(color).ToRGB().Invert();
    renderer->DrawRect(pos + Vec2f{sliderPos .x, 0}, Vec2f{1, size.h}, argb, false);
    renderer->DrawRect(pos + Vec2f{0, sliderPos .y}, Vec2f{size.w, 1}, argb, false);
  }

  void SatValSlider::OnEvent(Event& event, const Vec2f& componentPos)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      if(e.GetButton() == GREET_MOUSE_1 && pressed)
      {
        Vec2f translatedPos = e.GetPosition() - componentPos;
        SetSat(GetSliderSatFromPos(translatedPos.x));
        SetVal(GetSliderValFromPos(translatedPos.y));
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      if(pressed)
      {
        MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
        Vec2f translatedPos = e.GetPosition() - componentPos;
        SetSat(GetSliderSatFromPos(translatedPos.x));
        SetVal(GetSliderValFromPos(translatedPos.y));
      }
    }
  }

  void SatValSlider::SetOnSatChangeCallback(OnValueChangeCallback callback)
  {
    onSatChangeCallback = callback;
  }

  void SatValSlider::SetOnValChangeCallback(OnValueChangeCallback callback)
  {
    onValChangeCallback = callback;
  }

  void SatValSlider::CallOnSatChangeCallback(float oldSat, float newSat)
  {
    if(onSatChangeCallback)
      onSatChangeCallback(this, oldSat, newSat);
  }

  void SatValSlider::CallOnValChangeCallback(float oldVal, float newVal)
  {
    if(onValChangeCallback)
      onValChangeCallback(this, oldVal, newVal);
  }

  void SatValSlider::SetHue(float hue)
  {
    color.h = hue;
  }

  void SatValSlider::SetSat(float afSat, bool abCallback)
  {
    float oldSat = color.s;
    color.s = afSat;
    Math::Clamp(&color.s, 0.0f,1.0f);
    if(abCallback && color.s != oldSat)
      CallOnSatChangeCallback(oldSat, color.s);
  }

  void SatValSlider::SetVal(float afVal, bool abCallback)
  {
    float oldVal = color.v;
    color.v = afVal;
    Math::Clamp(&color.v, 0.0f,1.0f);
    if(abCallback && color.v != oldVal)
      CallOnValChangeCallback(oldVal, color.v);
  }

  float SatValSlider::GetSat() const
  {
    return color.s;
  }

  float SatValSlider::GetVal() const
  {
    return color.v;
  }

  float SatValSlider::GetSliderSatFromPos(float pos) const
  {
    return pos / (GetSize().w - 1);
  }

  float SatValSlider::GetSliderPosFromSat(float value) const
  {
    return (GetSize().w - 1) * value;
  }

  float SatValSlider::GetSliderValFromPos(float pos) const
  {
    return pos / (GetSize().h - 1);
  }

  float SatValSlider::GetSliderPosFromVal(float value) const
  {
    return (GetSize().h - 1) * value;
  }
}
