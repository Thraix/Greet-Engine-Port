#include "HueSlider.h"

#include <math/Maths.h>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(HueSlider);

  HueSlider::HueSlider(const std::string& name, Component* parent)
    : Slider{name, parent}
  {
    minValue = 0;
    maxValue = 1.0;
    SetValue(0.5);
  }

  HueSlider::HueSlider(const XMLObject& xmlObject, Component* parent)
    : Slider(xmlObject, parent)
  {
    minValue = 0;
    maxValue = 1.0;
    SetValue(0.5);
  }

  //void SubmitRect(const Vec2f& pos, const Vec2f& size, const Vec4& color1, const Vec4& color2, const Vec4& color3, const Vec4& color4, bool isHsv = true);
  void HueSlider::PreRender(GUIRenderer* renderer, const Vec2f& translation) const
  {
    renderer->PushTranslation(translation);
    renderer->DrawRect(pos, GetSize(), Color(0, 1, 1), Color(0, 1, 1), Color(1, 1, 1), Color(1, 1, 1), true);
  }
}
