#include "Button.h"

#include <graphics/fonts/FontManager.h>

namespace Greet {
  Button::Button(const XMLObject& object, Content* parent)
    : Content(object, parent)
  {
    textColor = Vec4(0.0,0.0,0.0,1.0);
    fontSize = 12;
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

  void Button::Render(GUIRenderer* renderer, const Vec2& position) const
  {
    Font* font = FontManager::Get("roboto",fontSize);
    renderer->SubmitString(xmlObject.GetText(), position+Vec2((GetWidth() - font->GetWidthOfText(xmlObject.GetText()))/2,GetHeight()/2), FontManager::Get("roboto",fontSize), textColor,false);
  }

  bool Button::IsFocusable() const
  {
    return true;
  }
}
