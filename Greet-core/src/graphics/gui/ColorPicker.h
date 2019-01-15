#pragma once

#include <graphics/gui/Component.h>
#include <graphics/gui/ColorPickerWindow.h>

namespace Greet
{
  class ColorPicker : public Component
  {
    public:
      typedef std::function<void(Component* comp, const Vec3& previous, const Vec3& current)> OnColorChangeCallback;
    public:
      OnColorChangeCallback onColorChangeCallback;
    private:
      ColorPickerWindow* picker;
    public:
      ColorPicker(const XMLObject& object, Component* parent);
      virtual ~ColorPicker();

      void MousePressed(const MousePressedEvent& e, const Vec2& translatedPos) override;
      void OnColorChanged(const Vec3& previous, const Vec3& current);

      void SetOnColorChangeCallback(OnColorChangeCallback callback);
      void CallOnColorChangeCallback(const Vec3& previous, const Vec3& current);
  };

}
