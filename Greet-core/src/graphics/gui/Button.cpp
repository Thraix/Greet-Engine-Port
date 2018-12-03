#include "Button.h"

#include <graphics/fonts/FontManager.h>

namespace Greet {
  Button::Button(const XMLObject& object, Component* parent)
    : Component(object, parent)
  {
    XMLObject labelObject;
    labelObject.SetName("Label");
    labelObject.SetText(object.GetText());

    labelObject.AddProperty("font", object.GetProperty("font"));
    labelObject.AddProperty("color", object.GetProperty("color"));
    labelObject.AddProperty("fontSize", object.GetProperty("fontSize"));
    labelObject.AddProperty("padding", "0px");
    labelObject.AddProperty("border", "0px");
    labelObject.AddProperty("margin", "0px");

    label = new Label(object, this);
    textColor = Vec4(0.0,0.0,0.0,1.0);
    m_isFocusable = true;
    fontSize = GUIUtils::GetSizeFromXML(object, "fontSize", 20, GetHeight());
    textColor = GUIUtils::GetColorFromXML(object, "fontColor", Vec4(0,0,0,1));
  }

  Button::~Button()
  {
    delete label;

  }

  void Button::Render(GUIRenderer* renderer) const
  {
    Vec2 p = pos + GetTotalPadding() +  Vec2(GetContentSize().w-label->GetWidth(), GetContentSize().h-label->GetFont()->GetBaselineOffset())/2;
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
