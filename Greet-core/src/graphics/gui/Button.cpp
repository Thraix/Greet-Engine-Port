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
    fontSize = 20;
    m_isFocusable = true;
    if (object.HasProperty("fontSize"))
    {
      fontSize = GUIUtils::CalcSize(object.GetProperty("fontSize"), GetHeight());
    }
    if (object.HasProperty("fontColor"))
    {
      textColor = GUIUtils::GetColor(object.GetProperty("fontColor"));
    }
  }

  Button::~Button()
  {

  }

  void Button::Render(GUIRenderer* renderer) const
  {
    label.Render(renderer);
  }
  void Button::SetPosition(const Vec2& pos)
  {
    Component::SetPosition(pos);
    label.SetPosition(pos + (size - label.GetSize())/2);
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
