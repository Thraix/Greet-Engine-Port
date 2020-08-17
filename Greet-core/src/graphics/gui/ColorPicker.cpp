#include "ColorPicker.h"

#include <functional>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(ColorPicker);

  ColorPicker::ColorPicker(const XMLObject& object, Component* parent)
    : Component{object, parent}, attachedColor{nullptr}
  {
    picker = new ColorPickerWindow(Vec2(0,0), Vec3<float>{GUIUtils::GetColorFromXML(object, "backgroundColor", Vec4{1,1,1,1})});
    using namespace std::placeholders;
    picker->SetOnColorChangeCallback(std::bind(&ColorPicker::OnColorChanged, std::ref(*this), _1, _2));
  }

  ColorPicker::~ColorPicker()
  {
    delete picker;
  }

  void ColorPicker::AttachColor(Vec3<float>* color)
  {
    attachedColor = color;
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
    if(attachedColor)
      *attachedColor = current;
    ComponentStyle& s = GetStyle("normal");
    s.SetColor("backgroundColor", Vec4(current.r,current.g,current.b,1));
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
