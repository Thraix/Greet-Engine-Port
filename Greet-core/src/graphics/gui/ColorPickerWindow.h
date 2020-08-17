#pragma once

#include <graphics/gui/Frame.h>
#include <graphics/gui/SatValSlider.h>
#include <graphics/gui/HueSlider.h>
#include <graphics/gui/TextBox.h>

namespace Greet
{
  class ColorPickerWindow : public Frame
  {
    protected:
      enum class InputChangeType
      {
        SLIDER, RGB_TEXTBOX, HSV_TEXTBOX, HEX_TEXTBOX
      };
    public:
      typedef std::function<void(const Vec3<float>& previous, const Vec3<float>& current)> OnColorChangeCallback;

    protected:
      static XMLObject frameLook;
      Vec3<float> color;
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

    protected:
      void SliderChanged(Component* component, float oldValue, float newValue);
      void RGBTextBoxChanged(Component* textBox, const std::string& oldText, const std::string& newText);
      void HSVTextBoxChanged(Component* textBox, const std::string& oldText, const std::string& newText);
      void HexTextBoxChanged(Component* textBox, const std::string& oldText, const std::string& newText);
      void UpdateColor(float hue, float sat, float val, InputChangeType type);

    public:
      ColorPickerWindow();
      ColorPickerWindow(const Vec2& pos, const Vec3<float>& color);

      void SetOnColorChangeCallback(OnColorChangeCallback callback);
      void CallOnColorChangeCallback(const Vec3<float>& previous, const Vec3<float>& current);
  };
}
