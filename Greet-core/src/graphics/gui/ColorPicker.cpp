#include "ColorPicker.h"

#include <functional>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(ColorPicker);

  ColorPicker::ColorPicker(const XMLObject& object, Component* parent)
    : Component{object, parent}
  {
    m_isFocusable = true;
    picker  = new ColorPickerWindow(Vec2(0,0), Vec3<float>(GetStyle("normal").backgroundColor));
    using namespace std::placeholders;
    picker->SetOnColorChangeCallback(std::bind(&ColorPicker::OnColorChanged, std::ref(*this), _1, _2));
  }

  ColorPicker::~ColorPicker()
  {
    delete picker;
  }

  void ColorPicker::OnEvent(Event& event, const Vec2& componentPos)
  {

    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      guiScene->AddFrame(picker);
      picker->SetPosition(e.GetPosition());
      guiScene->RequestFocus(picker);
    }
  }

  void ColorPicker::OnColorChanged(const Vec3<float>& previous, const Vec3<float>& current)
  {
    Style s = GetStyle("normal");
    s.SetBackgroundColor(Vec4(current.r,current.g,current.b,1));
    AddStyle("normal", s);
    CallOnColorChangeCallback(previous,current);
  }

  void ColorPicker::SetOnColorChangeCallback(OnColorChangeCallback callback)
  {
    onColorChangeCallback = callback;
  }

  void ColorPicker::CallOnColorChangeCallback(const Vec3<float>& previous, const Vec3<float>& current)
  {
    if(onColorChangeCallback)
      onColorChangeCallback(this, previous, current);
  }
}
