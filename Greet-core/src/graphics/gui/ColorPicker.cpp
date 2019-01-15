#include "ColorPicker.h"

#include <graphics/gui/GLayer.h>
#include <functional>

namespace Greet
{
  ColorPicker::ColorPicker(const XMLObject& object, Component* parent)
    : Component{object, parent}
  {
    m_isFocusable = true;

  }

  void ColorPicker::MousePressed(const MousePressedEvent& e, const Vec2& translatedPos)
  {
    ColorPickerWindow* window = new ColorPickerWindow(e.GetPosition(), GetNormalStyle().backgroundColor);
    GLayer::AddFrame(window);
    using namespace std::placeholders;
    window->SetOnColorChangeCallback(std::bind(&ColorPicker::OnColorChanged, std::ref(*this), _1, _2));
    GLayer::RequestFocus(window);
  }

  void ColorPicker::OnColorChanged(const Vec3& previous, const Vec3& current)
  {
    Style s = normalStyle;
    s.SetBackgroundColor(Vec4(current.r,current.g,current.b,1));
    SetNormalStyle(s);
    CallOnColorChangeCallback(previous,current);
  }

  void ColorPicker::SetOnColorChangeCallback(OnColorChangeCallback callback)
  {
    onColorChangeCallback = callback;
  }

  void ColorPicker::CallOnColorChangeCallback(const Vec3& previous, const Vec3& current)
  {
    if(onColorChangeCallback)
      onColorChangeCallback(this, previous, current);
  }
}
