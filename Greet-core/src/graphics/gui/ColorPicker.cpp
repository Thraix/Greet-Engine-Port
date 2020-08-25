#include "ColorPicker.h"

#include <functional>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(ColorPicker);

  ColorPicker::ColorPicker(const XMLObject& object, Component* parent)
    : Component{object, parent}, attachedColor{nullptr}
  {
    picker = new ColorPickerWindow(Vec2(0,0), Vec3<float>{GUIUtils::GetColorFromXML(object, "color", Vec4{1,1,1,1})});
    using namespace std::placeholders;
    picker->SetOnColorChangeCallback(std::bind(&ColorPicker::OnColorChanged, std::ref(*this), _1, _2));
  }

  ColorPicker::~ColorPicker()
  {
    delete picker;
  }

  void ColorPicker::PreRender(GUIRenderer* renderer, const Vec2& translation) const
  {
    renderer->PushTranslation(translation);

    // Border around Component
    if(borderColor.a != 0.0)
      //renderer->SubmitRect(pos + Vec2(0,0), size, currentStyle->borderColor, false);
      renderer->DrawRoundedRect(pos+Vec2(0,0), {width.size, height.size}, borderColor, borderRadius, borderRoundedPrecision, false);

    // Component background
    renderer->DrawRoundedRect(pos + border.LeftTop(), Vec2{width.size, height.size} - GetBorder().LeftTop()-GetBorder().RightBottom(), Vec4(GetColor().x, GetColor().y, GetColor().z, 1.0), backgroundRadius, backgroundRoundedPrecision, false);
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
    CallOnColorChangeCallback(previous,current);
  }

  void ColorPicker::SetOnColorChangeCallback(OnColorChangeCallback callback)
  {
    onColorChangeCallback = callback;
    CallOnColorChangeCallback({0.0f}, GetColor());
  }

  void ColorPicker::CallOnColorChangeCallback(const Vec3<float>& previous, const Vec3<float>& current)
  {
    if(onColorChangeCallback)
      onColorChangeCallback(this, previous, current);
  }

  const Vec3<float>& ColorPicker::GetColor() const
  {
    return picker->GetColor();
  }
}
