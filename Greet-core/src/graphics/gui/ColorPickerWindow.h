#pragma once

#include <graphics/gui/Frame.h>
#include <graphics/gui/SatValSlider.h>
#include <graphics/gui/HueSlider.h>
#include <graphics/gui/TextBox.h>

namespace Greet
{
  class ColorPickerWindow : public Frame
  {
    private:
      enum class InputChangeType
      {
        SLIDER, RGB_TEXTBOX, HSV_TEXTBOX, HEX_TEXTBOX
      };
    public:
      typedef std::function<void(const Vec3& previous, const Vec3& current)> OnColorChangeCallback;

    private:
      Vec3 color;
      HueSlider* hSlider;
      SatValSlider* svSlider;

      TextBox* rTextBox;
      TextBox* gTextBox;
      TextBox* bTextBox;
      TextBox* hTextBox;
      TextBox* sTextBox;
      TextBox* vTextBox;
      TextBox* hexTextBox;

      Component* colorDisplay;

      OnColorChangeCallback onColorChangeCallback;

    private:
      void SliderChanged();
      void RGBTextBoxChanged(Component* textBox);
      void HSVTextBoxChanged(Component* textBox);
      void HexTextBoxChanged();
      void UpdateColor(float hue, float sat, float val, InputChangeType type);

    public:
      ColorPickerWindow();
      ColorPickerWindow(const Vec2& pos, const Vec3& color);

      void SetOnColorChangeCallback(OnColorChangeCallback callback);
      void CallOnColorChangeCallback(const Vec3& previous, const Vec3& current);
  };
}
