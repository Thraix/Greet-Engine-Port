#include "Button.h"

#include <graphics/fonts/FontManager.h>

namespace Greet {
  Button::Button(const XMLObject& object, Component* parent)
    : Component(object, parent)
  {
    label = new Label("Label", this);
    label->SetFont(GUIUtils::GetStringFromXML(object, "font",""))
      .SetFontSize(GUIUtils::GetFloatFromXML(object,"fontSize",20))
      .SetText(object.GetText())
      .SetColor(GUIUtils::GetColorFromXML(object, "color", Vec4(0,0,0,1)))
      .SetSize(1,1,SizeType::WRAP, SizeType::WEIGHT);

    m_isFocusable = true;
    fontSize = GUIUtils::GetSizeFromXML(object, "fontSize", 20, GetHeight());
  }

  Button::~Button()
  {
    delete label;
  }

  void Button::OnMeasured()
  {
    label->Measure();
    label->MeasureFill(GetContentSize().w,GetContentSize().h,1,true);
  }

  void Button::Render(GUIRenderer* renderer) const
  {
    Vec2 p = pos + GetTotalPadding() +  Vec2(
        (GetContentSize().w-label->GetWidth())/2, 0);
    renderer->PushMatrix(Mat3::Translate(p));
    label->Render(renderer);
    renderer->PopMatrix();
  }

  void Button::OnFocused()
  {
  }

  void Button::OnUnfocused()
  {
  }

  void Button::MouseEntered()
  {
    currentStyle = &hoverStyle;
  }

  void Button::MouseExited()
  {
    currentStyle = &normalStyle;
  }
}
