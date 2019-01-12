#pragma once

#include <graphics/gui/Frame.h>
#include <graphics/gui/SatValSlider.h>
#include <graphics/gui/HueSlider.h>
#include <graphics/gui/TextBox.h>

namespace Greet
{
  class ColorPickerWindow : public Frame
  {
    public:
      typedef std::function<void(Component*, float oldValue, float newValue)> OnValueChangeCallback;
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

      void SliderChanged();
      void RGBTextBoxChanged();
      void HSVTextBoxChanged();
      void UpdateColor(float hue, float sat, float val);
      void SetOnTextChange(OnValueChangeCallback callback);

    public:
      ColorPickerWindow();
  };
}
