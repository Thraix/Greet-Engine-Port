#include "SatValSlider.h"

namespace Greet
{

  SatValSlider::SatValSlider(const XMLObject& xmlObject, Component* parent)
    : Component(xmlObject, parent)
  {
    m_isFocusable = true;
    if(xmlObject.GetObjectCount() > 0)
      sliderComponent = GUIUtils::GetComponent(xmlObject.GetObject(0), this);
  }

  void SatValSlider::PreRender(GUIRenderer* renderer, const Vec2& translation) const
  {
    renderer->PushMatrix(Mat3::Translate(translation));
    renderer->SubmitRect(pos, size, Vec4(1,0,0,1), Vec4(1,0,1,1), Vec4(1,1,0,1), Vec4(1,1,1,1),true);
  }

  void SatValSlider::Render(GUIRenderer* renderer) const
  {
    Vec2 sliderOffset = pos + GetTotalPadding() + sliderComponent->GetMargin().LeftTop() - sliderComponent->GetSize()/2;

    sliderComponent->PreRender(renderer,  sliderOffset + Vec2{sat,val} * GetContentSize());
    sliderComponent->RenderHandle(renderer);
    sliderComponent->PostRender(renderer);
  }

  void SatValSlider::MousePressed(const MousePressedEvent& event, const Vec2& translatedPos)
  {
    Component::MousePressed(event,translatedPos);
    if(event.GetButton() == GLFW_MOUSE_BUTTON_1 && pressed)
    {
      float oldSat = sat;
      float oldVal = val;

      SetSat(GetSliderSatFromPos(translatedPos.x));
      SetVal(GetSliderValFromPos(translatedPos.y));

      //if(oldSat != newSat || oldVal != newVal)
      //CallOnValueChangeCallback(oldSat, newSat, oldVal, newVal);
    }
  }

  void SatValSlider::MouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos)
  {
    if(pressed)
    {
      float oldSat = GetSliderSatFromPos(sat);
      float oldVal = GetSliderValFromPos(val);

      SetSat(GetSliderSatFromPos(translatedPos.x));
      SetVal(GetSliderValFromPos(translatedPos.y));

      if(oldSat != sat)
        CallOnSatChangeCallback(oldSat, sat);
      if(oldVal != val)
        CallOnValChangeCallback(oldVal, val);
    }
  }

  void SatValSlider::SetOnSatChangeCallback(OnSatChangeCallback callback)
  {
    onSatChangeCallback = callback;
  }

  void SatValSlider::SetOnValChangeCallback(OnValChangeCallback callback)
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

  void SatValSlider::SetSat(float sat)
  {
    this->sat= sat;
    Math::Clamp(&this->sat, 0.0f,1.0f);
  }

  void SatValSlider::SetVal(float val)
  {
    this->val = val;
    Math::Clamp(&this->val, 0.0f,1.0f);
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
    return pos / GetContentSize().w;
  }

  float SatValSlider::GetSliderPosFromSat(float value) const
  {
    return GetContentSize().w * value;
  }

  float SatValSlider::GetSliderValFromPos(float pos) const
  {
    return pos / GetContentSize().h;
  }

  float SatValSlider::GetSliderPosFromVal(float value) const
  {
    return GetContentSize().h * value;
  }
}
