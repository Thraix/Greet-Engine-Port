#include "ColorPickerWindow.h"

#include <graphics/gui/Label.h>
#include <functional>

namespace Greet
{
  ColorPickerWindow::ColorPickerWindow()
    : Frame{}
  {
    // Would be easier to create an XMLObject for this.
    // However that would need to be integrated with the engine and
    // I have no idea how to do this.

    Style s = Style{}.SetBackgroundColor(ColorUtils::ColorHexToVec4(0xff263238))
      .SetBorderColor(ColorUtils::ColorHexToVec4(0xff37474f))
      .SetBorder({1,1,1,1})
      .SetPadding({10,10,10,10});

    SetVertical(false)
      .SetNormalStyle(s)
      .SetSize(1,1, SizeType::WRAP, SizeType::WRAP,false);

    svSlider = new SatValSlider("", this);
    svSlider->SetSize(200,200,SizeType::NONE,SizeType::NONE,false);
    AddComponent(svSlider);

    hSlider = new HueSlider("", this);
    hSlider->SetVertical(true)
      .SetSize(20,1, SizeType::NONE, SizeType::WEIGHT, false);
    hSlider->GetSliderComponent()->SetSize(1,7,SizeType::WEIGHT, SizeType::NONE);
    AddComponent(hSlider);

    Container* textBoxContainer = new Container("",this);
    textBoxContainer->SetSize(110,1,SizeType::NONE, SizeType::WEIGHT);
    AddComponent(textBoxContainer);

    Style textBoxStyle = Style{}.SetPadding({0, 5, 0, 5})
      .SetRadius(3)
      .SetBorder({1,1,1,1})
      .SetBorderRadius(4)
      .SetBackgroundColor(ColorUtils::ColorHexToVec4(0xff37474f))
      .SetBorderColor(ColorUtils::ColorHexToVec4(0xff455a64));

    {
      Container* rgbhsvContainer = new Container("",this);
      rgbhsvContainer->SetVertical(false).SetSize(1,1,SizeType::WEIGHT, SizeType::WRAP);
      textBoxContainer->AddComponent(rgbhsvContainer);

      {
        Container* rgbContainer = new Container("",this);
        rgbContainer->SetSpacing(5).SetVertical(false).SetSize(1,1,SizeType::WEIGHT, SizeType::WRAP);
        rgbhsvContainer->AddComponent(rgbContainer);

        {
          Label* text = new Label("",this);
          text->SetText("R")
            .SetFontSize(16)
            .SetFont("roboto")
            .SetSize(1,1,SizeType::WRAP, SizeType::WEIGHT);
          rgbContainer->AddComponent(text);

          rTextBox = new TextBox("",this);

          rTextBox->SetFont("roboto")
            .SetFontSize(16)
            .SetSize(1,20, SizeType::WEIGHT, SizeType::NONE)
            .SetNormalStyle(textBoxStyle);

          rgbContainer->AddComponent(rTextBox);
        }

      }

      Container* hsvContainer = new Container("",this);
      hsvContainer->SetSpacing(5).SetVertical(false).SetSize(1,1,SizeType::WEIGHT, SizeType::WRAP);
      rgbhsvContainer->AddComponent(hsvContainer);
      {
        Label* text = new Label("",this);
        text->SetText("H")
          .SetFontSize(16)
          .SetFont("roboto")
          .SetSize(1,1,SizeType::WRAP, SizeType::WEIGHT);
        hsvContainer->AddComponent(text);

        hTextBox = new TextBox("",this);

        hTextBox->SetFont("roboto")
          .SetFontSize(16)
          .SetSize(1,20, SizeType::WEIGHT, SizeType::NONE)
          .SetNormalStyle(textBoxStyle);

        hsvContainer->AddComponent(hTextBox);
      }
    }
    {
      Container* rgbhsvContainer = new Container("",this);
      rgbhsvContainer->SetVertical(false).SetSize(1,1,SizeType::WEIGHT, SizeType::WRAP);
      textBoxContainer->AddComponent(rgbhsvContainer);

      {
        Container* rgbContainer = new Container("",this);
        rgbContainer->SetSpacing(5).SetVertical(false).SetSize(1,1,SizeType::WEIGHT, SizeType::WRAP);
        rgbhsvContainer->AddComponent(rgbContainer);

        {
          Label* text = new Label("",this);
          text->SetText("G")
            .SetFontSize(16)
            .SetFont("roboto")
            .SetSize(1,1,SizeType::WRAP, SizeType::WEIGHT);
          rgbContainer->AddComponent(text);

          gTextBox = new TextBox("",this);

          gTextBox->SetFont("roboto")
            .SetFontSize(16)
            .SetSize(1,20, SizeType::WEIGHT, SizeType::NONE)
            .SetNormalStyle(textBoxStyle);

          rgbContainer->AddComponent(gTextBox);
        }

      }

      Container* hsvContainer = new Container("",this);
      hsvContainer->SetSpacing(5).SetVertical(false).SetSize(1,1,SizeType::WEIGHT, SizeType::WRAP);
      rgbhsvContainer->AddComponent(hsvContainer);
      {
        Label* text = new Label("",this);
        text->SetText("S")
          .SetFontSize(16)
          .SetFont("roboto")
          .SetSize(1,1,SizeType::WRAP, SizeType::WEIGHT);
        hsvContainer->AddComponent(text);

        sTextBox = new TextBox("",this);

        sTextBox->SetFont("roboto")
          .SetFontSize(16)
          .SetSize(1,20, SizeType::WEIGHT, SizeType::NONE)
          .SetNormalStyle(textBoxStyle);

        hsvContainer->AddComponent(sTextBox);
      }
    }
    {
      Container* rgbhsvContainer = new Container("",this);
      rgbhsvContainer->SetVertical(false).SetSize(1,1,SizeType::WEIGHT, SizeType::WRAP);
      textBoxContainer->AddComponent(rgbhsvContainer);

      {
        Container* rgbContainer = new Container("",this);
        rgbContainer->SetSpacing(5).SetVertical(false).SetSize(1,1,SizeType::WEIGHT, SizeType::WRAP);
        rgbhsvContainer->AddComponent(rgbContainer);

        {
          Label* text = new Label("",this);
          text->SetText("B")
            .SetFontSize(16)
            .SetFont("roboto")
            .SetSize(1,1,SizeType::WRAP, SizeType::WEIGHT);
          rgbContainer->AddComponent(text);

          bTextBox = new TextBox("",this);

          bTextBox->SetFont("roboto")
            .SetFontSize(16)
            .SetSize(1,20, SizeType::WEIGHT, SizeType::NONE)
            .SetNormalStyle(textBoxStyle);

          rgbContainer->AddComponent(bTextBox);
        }

      }

      Container* hsvContainer = new Container("",this);
      hsvContainer->SetSpacing(5).SetVertical(false).SetSize(1,1,SizeType::WEIGHT, SizeType::WRAP);
      rgbhsvContainer->AddComponent(hsvContainer);
      {
        Label* text = new Label("",this);
        text->SetText("V")
          .SetFontSize(16)
          .SetFont("roboto")
          .SetSize(1,1,SizeType::WRAP, SizeType::WEIGHT);
        hsvContainer->AddComponent(text);

        vTextBox = new TextBox("",this);

        vTextBox->SetFont("roboto")
          .SetFontSize(16)
          .SetSize(1,20, SizeType::WEIGHT, SizeType::NONE)
          .SetNormalStyle(textBoxStyle);

        hsvContainer->AddComponent(vTextBox);
      }
    }

    Container* hexContainer = new Container("",this);
    hexContainer->SetSpacing(5).SetVertical(false).SetSize(1,1,SizeType::WEIGHT, SizeType::WRAP);
    textBoxContainer->AddComponent(hexContainer);
    {
      Label* text = new Label("",this);
      text->SetText("#")
        .SetFontSize(16)
        .SetFont("roboto")
        .SetSize(1,1,SizeType::WRAP, SizeType::WEIGHT);
      hexContainer->AddComponent(text);

      hexTextBox = new TextBox("",this);

      hexTextBox->SetFont("roboto")
        .SetFontSize(16)
        .SetSize(1,20, SizeType::WEIGHT, SizeType::NONE)
        .SetNormalStyle(textBoxStyle);

      hexContainer->AddComponent(hexTextBox);
    }
    colorDisplay = new Component("",this);
    Style colorStyle = Style{}.SetBackgroundColor(ColorUtils::ColorHexToVec4(0xff37474f))
      .SetBorderColor(ColorUtils::ColorHexToVec4(0xff455a64))
      .SetBorder({1,1,1,1});
    colorDisplay->SetSize(1,1,SizeType::WEIGHT, SizeType::WEIGHT)
      .SetNormalStyle(colorStyle);
    textBoxContainer->AddComponent(colorDisplay);

    using namespace std::placeholders;

    hSlider->SetOnValueChangeCallback(std::bind(&ColorPickerWindow::SliderChanged, std::ref(*this)));
    svSlider->SetOnSatChangeCallback(std::bind(&ColorPickerWindow::SliderChanged, std::ref(*this)));
    svSlider->SetOnValChangeCallback(std::bind(&ColorPickerWindow::SliderChanged, std::ref(*this)));
    rTextBox->SetOnTextChangedCallback(std::bind(&ColorPickerWindow::RGBTextBoxChanged, std::ref(*this),_1));
    gTextBox->SetOnTextChangedCallback(std::bind(&ColorPickerWindow::RGBTextBoxChanged, std::ref(*this),_1));
    bTextBox->SetOnTextChangedCallback(std::bind(&ColorPickerWindow::RGBTextBoxChanged, std::ref(*this),_1));
    hTextBox->SetOnTextChangedCallback(std::bind(&ColorPickerWindow::HSVTextBoxChanged, std::ref(*this),_1));
    sTextBox->SetOnTextChangedCallback(std::bind(&ColorPickerWindow::HSVTextBoxChanged, std::ref(*this),_1));
    vTextBox->SetOnTextChangedCallback(std::bind(&ColorPickerWindow::HSVTextBoxChanged, std::ref(*this),_1));
    hexTextBox->SetOnTextChangedCallback(std::bind(&ColorPickerWindow::HexTextBoxChanged, std::ref(*this)));

    // Make textboxes and other stuff update
    SliderChanged();
  }

  void ColorPickerWindow::UpdateColor(float hue, float sat, float val, InputChangeType type)
  {
    Style s = colorDisplay->GetNormalStyle();
    Vec4 rgb = ColorUtils::HSVtoRGB(hue,sat,val,1);
    s.SetBackgroundColor(rgb);
    colorDisplay->SetNormalStyle(s);
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
  }

  void ColorPickerWindow::SliderChanged()
  {
    UpdateColor(hSlider->GetValue(), svSlider->GetSat(),svSlider->GetVal(), InputChangeType::SLIDER);
  }

  void ColorPickerWindow::RGBTextBoxChanged(Component* textBox)
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

  void ColorPickerWindow::HSVTextBoxChanged(Component* textBox)
  {
    float h = atoi(hTextBox->GetText().c_str())/255.0f;
    float s = atoi(sTextBox->GetText().c_str())/255.0f;
    float v = atoi(vTextBox->GetText().c_str())/255.0f;
    Math::Clamp(&h,0.0f,1.0f);
    Math::Clamp(&s,0.0f,1.0f);
    Math::Clamp(&v,0.0f,1.0f);
    UpdateColor(h,s,v,InputChangeType::HSV_TEXTBOX);
  }

  void ColorPickerWindow::HexTextBoxChanged()
  {
    Vec4 rgba = ColorUtils::HexToVec4(LogUtils::HexToDec(hexTextBox->GetText()));
    Vec4 hsv = ColorUtils::RGBtoHSV(rgba.r,rgba.g,rgba.b,1);
    UpdateColor(hsv.h,hsv.s,hsv.v,InputChangeType::HEX_TEXTBOX);
  }
}
