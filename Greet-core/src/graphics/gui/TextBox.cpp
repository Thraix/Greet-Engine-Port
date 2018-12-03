#include "TextBox.h"
#include <utils/StringUtils.h>

namespace Greet
{
  TextBox::TextBox(const XMLObject& object, Component* parent)
    : Component(object,parent), cursorPos(0), selectionPos(0)
  {
    m_isFocusable = true;
    XMLObject labelObject;
    labelObject.SetName("Label");
    labelObject.SetText(object.GetText());

    labelObject.AddProperty("font", object.GetProperty("font"));
    labelObject.AddProperty("color", object.GetProperty("color"));
    labelObject.AddProperty("fontSize", object.GetProperty("fontSize"));

    text = Label(object, this);
    text.SetPosition(pos);

  }

  void TextBox::Render(GUIRenderer* renderer) const
  {
    renderer->PushMatrix(Mat3::Translate(pos + Vec2(GetTotalPadding().w,GetContentSize().h/2)));
    text.Render(renderer);
    renderer->PopMatrix();

    if(isFocused && cursorBlinkTimer < 0.5)
    {
      float p = text.GetFont()->GetWidthOfText(text.GetText(),0, cursorPos);
      Vec2 curPos = pos + GetTotalPadding() + Vec2(p, (GetContentSize().h - text.GetHeight())/2);
      renderer->SubmitRect(curPos,Vec2(1,text.GetHeight()),text.GetColor(),false);
    }
  }

  void TextBox::Update(float timeElapsed)
  {
    cursorBlinkTimer += timeElapsed;
    if(cursorBlinkTimer >= 1)
      cursorBlinkTimer -= 1;
  }

  void TextBox::KeyTyped(const KeyTypedEvent& event)
  {
    std::string str = text.GetText();
    auto it = str.begin();
    std::advance(it, cursorPos);
    str.insert(it, (char)event.GetCharCode());
    text.SetText(str);
    cursorPos++;
  }  

  void TextBox::KeyPressed(const KeyPressedEvent& event)
  {
    // Easier to type if the blinker is turned on while typing
    cursorBlinkTimer = 0;
    if(event.GetButton() == GLFW_KEY_LEFT_CONTROL)
    {
      ctrlDown = true;
    }
    else if(event.GetButton() == GLFW_KEY_LEFT_CONTROL)
    {
      if(cursorPos > 0)
      {
        std::string str = text.GetText();
        str.erase(cursorPos-1,1);
        text.SetText(str);
        cursorPos--;
      }
    }
    else if(event.GetButton() == GLFW_KEY_LEFT)
    {
      if(ctrlDown)
        MoveCursorWord(false);
      else
        MoveCursor(-1);
    }
    else if(event.GetButton() == GLFW_KEY_RIGHT)
    {
      if(ctrlDown)
        MoveCursorWord(true);
      else
        MoveCursor(1);
    }
    else if(event.GetButton() == GLFW_KEY_BACKSPACE)
    {
      if(cursorPos > 0)
      {
        std::string str = text.GetText();
        if(ctrlDown)
        {
          int lastCursorPos = cursorPos;
          MoveCursorWord(false);
          str.erase(cursorPos,lastCursorPos-cursorPos);
          text.SetText(str);
        }
        else
        {
          str.erase(cursorPos-1,1);
          text.SetText(str);
          cursorPos--;
        }
      }
    }
    else if(event.GetButton() == GLFW_KEY_HOME)
    {
      cursorPos = 0;
    }
    else if(event.GetButton() == GLFW_KEY_END)
    {
      cursorPos = text.GetText().length();
    }
  }
  void TextBox::KeyReleased(const KeyReleasedEvent& event)
  {
    if(event.GetButton() == GLFW_KEY_LEFT_CONTROL)
    {
      ctrlDown = false;
    }
  }

  const std::string& TextBox::GetText() const
  {
    return text.GetText();
  }

  void TextBox::SetText(const std::string& text)
  {
    this->text.SetText(text);
  }

  void TextBox::MoveCursor(int delta)
  {
    cursorPos += delta;
    Math::Clamp<int>(&cursorPos, 0, text.GetText().length());
  }

  void TextBox::MoveCursorWord(bool forward)
  {
    int delta = forward ? 1 : -1;
    // Offset the cursor to make it easier to use with delta

    if(!forward)
      cursorPos+=delta;
    StringUtils::CharType type = StringUtils::GetCharType(text.GetText()[cursorPos]);
    while(type == StringUtils::GetCharType(text.GetText()[cursorPos]) && cursorPos >= 0 && cursorPos < text.GetText().length())
    {
      cursorPos += delta;
    }
    Math::Clamp<int>(&cursorPos, 0, text.GetText().length());
    if(type != StringUtils::GetCharType(text.GetText()[cursorPos]) && !forward)
      cursorPos -= delta;

  }

  int TextBox::GetCursorPos() const
  {
    return cursorPos;

  }
  int TextBox::GetSelectionPos() const
  {
    return selectionPos;

  }
}
