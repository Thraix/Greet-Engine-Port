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

    label = Label(object, this);
    label.SetPosition(pos);
    textColor = Vec4(0.0,0.0,0.0,1.0);
    m_isFocusable = true;
    fontSize = GUIUtils::GetSizeFromXML(object, "fontSize", 20, GetHeight());
    textColor = GUIUtils::GetColorFromXML(object, "fontColor", Vec4(0,0,0,1));
  }

  Button::~Button()
  {

  }

  void Button::Render(GUIRenderer* renderer) const
  {
    renderer->PushMatrix(Mat3::Translate(pos + (size - label.GetSize())/2));
    label.Render(renderer);
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
