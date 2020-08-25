#include "ColorPickerWindow.h"

#include <graphics/gui/Label.h>
#include <utils/LogUtils.h>

#include <functional>
#include <sstream>

namespace Greet
{
  XMLObject ColorPickerWindow::frameLook = XMLObject
  {
#include "ColorPickerLook.xml"
  };

  MetaFile ColorPickerWindow::frameStyle = MetaFile
  {
    std::istringstream(
#include "ColorPickerStyle.meta"
    )
  };

  ColorPickerWindow::ColorPickerWindow(const Vec2& pos, const Vec3<float>& color)
    : Frame(frameLook)
  {
    LoadFrameStyle(frameStyle);
    shouldCloseUnfocus = true;
    LoadStyles(frameLook);
    Measure({}, {});

    Vec3<float> hsv(ColorUtils::RGBtoHSV(Vec4(color.r, color.g, color.b, 1)));
    svSlider = GetComponentByName<SatValSlider>("CPW#SatValSlider");
    svSlider->SetHue(hsv.h);
    svSlider->SetSat(hsv.s);
    svSlider->SetVal(hsv.v);

    hSlider = GetComponentByName<HueSlider>("CPW#HueSlider");
    hSlider->GetSliderComponent()->SetSize(1,7, GUISize::Type::Weight, GUISize::Type::Pixels);
    hSlider->SetValue(hsv.h);
    rTextBox = GetComponentByName<TextBox>("CPW#RTextBox");
    gTextBox = GetComponentByName<TextBox>("CPW#GTextBox");
    bTextBox = GetComponentByName<TextBox>("CPW#BTextBox");
    hTextBox = GetComponentByName<TextBox>("CPW#HTextBox");
    sTextBox = GetComponentByName<TextBox>("CPW#STextBox");
    vTextBox = GetComponentByName<TextBox>("CPW#VTextBox");
    hexTextBox = GetComponentByName<TextBox>("CPW#HexTextBox");
    colorDisplay = GetComponentByName<Component>("CPW#ColorDisplay");

    using namespace std::placeholders;

    hSlider->SetOnValueChangeCallback(BIND_MEMBER_FUNC(SliderChanged));
    svSlider->SetOnSatChangeCallback(BIND_MEMBER_FUNC(SliderChanged));
    svSlider->SetOnValChangeCallback(BIND_MEMBER_FUNC(SliderChanged));
    rTextBox->SetOnTextChangedCallback(BIND_MEMBER_FUNC(RGBTextBoxChanged));
    gTextBox->SetOnTextChangedCallback(BIND_MEMBER_FUNC(RGBTextBoxChanged));
    bTextBox->SetOnTextChangedCallback(BIND_MEMBER_FUNC(RGBTextBoxChanged));
    hTextBox->SetOnTextChangedCallback(BIND_MEMBER_FUNC(HSVTextBoxChanged));
    sTextBox->SetOnTextChangedCallback(BIND_MEMBER_FUNC(HSVTextBoxChanged));
    vTextBox->SetOnTextChangedCallback(BIND_MEMBER_FUNC(HSVTextBoxChanged));
    hexTextBox->SetOnTextChangedCallback(BIND_MEMBER_FUNC(HexTextBoxChanged));

    // Make textboxes and other stuff update
    UpdateColor(hSlider->GetValue(), svSlider->GetSat(),svSlider->GetVal(), InputChangeType::SLIDER);
    SetPosition(pos);

  }
  ColorPickerWindow::ColorPickerWindow()
    : ColorPickerWindow {{0,0}, {1,1,1}}
  {
  }

  void ColorPickerWindow::UpdateColor(float hue, float sat, float val, InputChangeType type)
  {
    ComponentStyle& s = colorDisplay->GetStyle("normal");
    Vec3<float> prevRGB = color;
    Vec4 rgb = ColorUtils::HSVtoRGB(hue,sat,val,1);
    color = Vec3<float>(rgb);
    s.SetColor("backgroundColor", rgb);
    svSlider->SetHue(hue);

    if(type != InputChangeType::RGB_TEXTBOX)
    {
      rTextBox->SetText(std::to_string((int)(255*rgb.r)));
      gTextBox->SetText(std::to_string((int)(255*rgb.g)));
      bTextBox->SetText(std::to_string((int)(255*rgb.b)));
    }
    if(type != InputChangeType::HSV_TEXTBOX)
    {
      hTextBox->SetText(std::to_string((int)(255*hue)));
      sTextBox->SetText(std::to_string((int)(255*sat)));
      vTextBox->SetText(std::to_string((int)(255*val)));
    }
    if(type != InputChangeType::SLIDER)
    {
      hSlider->SetValue(hue);
      svSlider->SetSat(sat);
      svSlider->SetVal(val);
    }
    if(type != InputChangeType::HEX_TEXTBOX)
    {
      hexTextBox->SetText(LogUtils::DecToHex(((int)(255*rgb.r) << 16) | ((int)(255*rgb.g) << 8)  | (int)(255*rgb.b),6));
    }
    if(Vec3<float>(rgb) != prevRGB)
      CallOnColorChangeCallback(prevRGB, Vec3<float>(rgb));
  }

  void ColorPickerWindow::SliderChanged(Component* component, float oldValue, float newValue)
  {
    UpdateColor(hSlider->GetValue(), svSlider->GetSat(),svSlider->GetVal(), InputChangeType::SLIDER);
  }

  void ColorPickerWindow::RGBTextBoxChanged(Component* textBox, const std::string& oldText, const std::string& newText)
  {
    float r = atof(rTextBox->GetText().c_str())/255.0f;
    float g = atof(gTextBox->GetText().c_str())/255.0f;
    float b = atof(bTextBox->GetText().c_str())/255.0f;

    Math::Clamp(&r,0.0f,1.0f);
    Math::Clamp(&g,0.0f,1.0f);
    Math::Clamp(&b,0.0f,1.0f);

    Vec4 hsv = ColorUtils::RGBtoHSV(r,g,b,1);
    UpdateColor(hsv.h,hsv.s,hsv.v,InputChangeType::RGB_TEXTBOX);
  }

  void ColorPickerWindow::HSVTextBoxChanged(Component* textBox, const std::string& oldText, const std::string& newText)
  {
    float h = atoi(hTextBox->GetText().c_str())/255.0f;
    float s = atoi(sTextBox->GetText().c_str())/255.0f;
    float v = atoi(vTextBox->GetText().c_str())/255.0f;
    Math::Clamp(&h,0.0f,1.0f);
    Math::Clamp(&s,0.0f,1.0f);
    Math::Clamp(&v,0.0f,1.0f);
    UpdateColor(h,s,v,InputChangeType::HSV_TEXTBOX);
  }

  void ColorPickerWindow::HexTextBoxChanged(Component* textBox, const std::string& oldText, const std::string& newText)
  {
    Vec4 rgba = ColorUtils::HexToVec4(LogUtils::HexToDec(hexTextBox->GetText()));
    Vec4 hsv = ColorUtils::RGBtoHSV(rgba.r,rgba.g,rgba.b,1);
    UpdateColor(hsv.h,hsv.s,hsv.v,InputChangeType::HEX_TEXTBOX);
  }

  void ColorPickerWindow::SetOnColorChangeCallback(OnColorChangeCallback callback)
  {
    onColorChangeCallback = callback;
  }

  void ColorPickerWindow::CallOnColorChangeCallback(const Vec3<float>& previous, const Vec3<float>& current)
  {
    if(onColorChangeCallback)
      onColorChangeCallback(previous, current);
  }

  const Vec3<float>& ColorPickerWindow::GetColor() const
  {
    return color;
  }
}
