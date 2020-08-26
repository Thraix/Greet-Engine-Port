#include "SatValSlider.h"

#include <input/InputDefines.h>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(SatValSlider);

  SatValSlider::SatValSlider(const std::string& name, Component* parent, const std::string& componentType)
    : Component{name, parent, componentType}, hue{0}, sat{0.5}, val{0.5}
  {}

  SatValSlider::SatValSlider(const XMLObject& xmlObject, Component* parent)
    : Component(xmlObject, parent)
  {}

  SatValSlider::~SatValSlider()
  {}

  void SatValSlider::PreRender(GUIRenderer* renderer, const Vec2& translation) const
  {
    renderer->PushTranslation(translation);
    renderer->DrawRect(pos, GetSize(), Vec4(hue, 0, 0, 1), Vec4(hue, 1, 0, 1), Vec4(hue, 0, 1, 1), Vec4(hue, 1, 1, 1), true);
  }

  void SatValSlider::Render(GUIRenderer* renderer) const
  {
    Vec2 sliderPos = {GetSliderPosFromSat(sat), GetSliderPosFromVal(val)};
    Vec2 size = GetSize();
    Vec4 color = 1 - ColorUtils::HSVtoRGB(Vec4{hue, sat, val, 0});
    renderer->DrawRect(pos + Vec2{sliderPos .x, 0}, Vec2{1, size.h}, color, false);
    renderer->DrawRect(pos + Vec2{0, sliderPos .y}, Vec2{size.w, 1}, color, false);
  }

  void SatValSlider::OnEvent(Event& event, const Vec2& componentPos)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      if(e.GetButton() == GREET_MOUSE_1 && pressed)
      {
        Vec2 translatedPos = e.GetPosition() - componentPos;
        SetSat(GetSliderSatFromPos(translatedPos.x));
        SetVal(GetSliderValFromPos(translatedPos.y));
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      if(pressed)
      {
        MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
        Vec2 translatedPos = e.GetPosition() - componentPos;
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
    this->hue = hue;
  }

  void SatValSlider::SetSat(float afSat)
  {
    float oldSat = sat;
    sat = afSat;
    Math::Clamp(&this->sat, 0.0f,1.0f);
    if(sat != oldSat)
      CallOnSatChangeCallback(oldSat, sat);
  }

  void SatValSlider::SetVal(float afVal)
  {
    float oldVal = val;
    val = afVal;
    Math::Clamp(&this->val, 0.0f,1.0f);
    if(val != oldVal)
      CallOnValChangeCallback(oldVal, val);
  }

  float SatValSlider::GetSat() const
  {
    return sat;
  }

  float SatValSlider::GetVal() const
  {
    return val;
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
