#include "ColorPicker.h"

#include <graphics/gui/GLayer.h>
#include <functional>

namespace Greet
{
  ColorPicker::ColorPicker(const XMLObject& object, Component* parent)
    : Component{object, parent}
  {
    m_isFocusable = true;
    picker  = new ColorPickerWindow(Vec2(0,0), GetNormalStyle().backgroundColor);
    using namespace std::placeholders;
    picker->SetOnColorChangeCallback(std::bind(&ColorPicker::OnColorChanged, std::ref(*this), _1, _2));
  }

  ColorPicker::~ColorPicker()
  {
    delete picker;
  }

  void ColorPicker::MousePressed(const MousePressedEvent& e, const Vec2& translatedPos)
  {
    GLayer::AddFrame(picker);
    picker->SetPosition(e.GetPosition());
    GLayer::RequestFocus(picker);
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
