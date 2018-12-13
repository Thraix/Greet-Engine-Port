#include "TextBox.h"

#include <utils/StringUtils.h>

namespace Greet
{
  TextBox::TextBox(const XMLObject& object, Component* parent)
    : Component(object,parent), cursorPos(0), selectionPos(0), cursorBlinkTimer(0), textOffset{0}

  {
    m_isFocusable = true;
    XMLObject labelObject;
    labelObject.SetName("Label");
    labelObject.AddProperty("font", GUIUtils::GetStringFromXML(object, "font",""));
    labelObject.AddProperty("color", GUIUtils::GetStringFromXML(object, "color","#000000"));
    labelObject.AddProperty("fontSize", GUIUtils::GetStringFromXML(object, "fontSize","20px"));
    labelObject.AddProperty("padding", "0px");

    XMLObject hintLabelObject;
    hintLabelObject.AddProperty("font", 
        GUIUtils::GetStringFromXML(object, "hintFont",labelObject.GetProperty("font")));
    hintLabelObject.AddProperty("color", 
        GUIUtils::GetStringFromXML(object, "hintColor",labelObject.GetProperty("color")));
    hintLabelObject.AddProperty("fontSize", 
        GUIUtils::GetStringFromXML(object, "hintFontSize",labelObject.GetProperty("fontSize")));
    hintLabelObject.AddProperty("padding", "0px");

    text = new Label(labelObject, this);
    hintText = new Label(hintLabelObject, this);

    password = GUIUtils::GetBooleanFromXML(object, "password", false);
    SetText(object.GetText());
    hintText->SetText(GUIUtils::GetStringFromXML(object, "hintText", ""));
  }

  TextBox::~TextBox()
  {
    delete text;
    delete hintText;
  }

  void TextBox::Render(GUIRenderer* renderer) const
  {
    renderer->PushViewport(pos+GetTotalPadding(), GetContentSize(), true);
    if(text->GetText().length() == 0)
    {
      Vec2 p = pos + GetTotalPadding() +  Vec2(-textOffset, (GetContentSize().h-text->GetFont()->GetBaselineOffset())/2);
      renderer->PushMatrix(Mat3::Translate(p));
      hintText->Render(renderer);
    }
    else
    {
      Vec2 p = pos + GetTotalPadding() +  Vec2(-textOffset, (GetContentSize().h-hintText->GetFont()->GetBaselineOffset())/2);
      renderer->PushMatrix(Mat3::Translate(p));
      text->Render(renderer);
    }

    renderer->PopViewport();
    renderer->PopMatrix();

    if(isFocused && cursorBlinkTimer < 0.5)
    {
      float p = text->GetFont()->GetWidthOfText(text->GetText(),0, cursorPos)-textOffset;
      Vec2 curPos = pos + GetTotalPadding() + Vec2(p, (GetContentSize().h - text->GetHeight())/2);
      renderer->SubmitRect(curPos,Vec2(1,text->GetHeight()),text->GetColor(),false);
    }
  }

  void TextBox::Update(float timeElapsed)
  {
    cursorBlinkTimer += timeElapsed;
    if(cursorBlinkTimer >= 1)
      cursorBlinkTimer -= 1;
  }

  void TextBox::MousePressed(const MousePressedEvent& event, const Vec2& translatedPos)
  {
    if(event.GetButton() == GLFW_MOUSE_BUTTON_1)
    {
      // TODO: In the future we probably want to do some smart, average character length
      // to determain around where the cursor should be.
      std::vector<float> widths{text->GetFont()->GetPartialWidths(str)};
      auto it{widths.begin()};
      float w = -textOffset + GetTotalPadding().w;
      uint index = 0;
      while(it != widths.end() && w+*it <= translatedPos.x)
      {
        ++it; 
        index++;
      }
      // The cursor should be at the cursor behind
      // Maybe change to the nearest cursor?
      index--;
      MoveCursor(index - cursorPos);
      cursorBlinkTimer = 0;
    }
  }

  void TextBox::KeyTyped(const KeyTypedEvent& event)
  {
    auto it = str.begin();
    std::advance(it, cursorPos);
    str.insert(it, (char)event.GetCharCode());
    SetText(str);
    MoveCursor(1);
  }  

  void TextBox::KeyPressed(const KeyPressedEvent& event)
  {
    // Easier to type if the blinker is turned on while typing
    cursorBlinkTimer = 0;
    if(event.GetButton() == GLFW_KEY_LEFT_CONTROL)
    {
      ctrlDown = true;
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
        if(ctrlDown)
        {
          int lastCursorPos = cursorPos;
          MoveCursorWord(false);
          RemoveText(cursorPos, lastCursorPos - cursorPos);
          RecenterText();
        }
        else
        {
          RemoveText(cursorPos-1,1);
          MoveCursor(-1);
        }
      }
    }
    else if(event.GetButton() == GLFW_KEY_DELETE)
    {
      if(cursorPos < str.length())
      {
        if(ctrlDown)
        {
          int lastCursorPos = cursorPos;

          // Save the last offset since we dont want MoveCursor to update it
          MoveCursorWord(true);

          RemoveText(lastCursorPos,cursorPos - lastCursorPos);
          cursorPos = lastCursorPos;
          RecenterText();
        }
        else
        {
          RemoveText(cursorPos,1);
        }
      }
    }
    else if(event.GetButton() == GLFW_KEY_HOME)
    {
      MoveCursor(-cursorPos);
    }
    else if(event.GetButton() == GLFW_KEY_END)
    {
      MoveCursor(text->GetText().length()-cursorPos);
    }
    else if(event.GetButton() == GLFW_KEY_PAGE_UP)
    {
      // TODO: When multiline is implemented move to top of page
      MoveCursor(-cursorPos);
    }
    else if(event.GetButton() == GLFW_KEY_PAGE_DOWN)
    {
      // TODO: When multiline is implemented move to bottom of page
      MoveCursor(text->GetText().length()-cursorPos);
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
    return text->GetText();
  }

  void TextBox::SetText(const std::string& text)
  {
    if(password)
      this->text->SetText(StringUtils::Passwordify(text));
    else
      this->text->SetText(text);
    str = text;
  }

  void TextBox::RemoveText(uint start, uint n = -1)
  {
    if(n == -1)
      n = str.length()-start;
    str.erase(start,n);
    SetText(str);
  }

  void TextBox::RecenterText()
  {
    float cursorTextWidth = text->GetFont()->GetWidthOfText(text->GetText(),0,cursorPos);
    float textWidth = text->GetFont()->GetWidthOfText(text->GetText());
    if(cursorTextWidth - textOffset > GetContentSize().w)
    {
      textOffset = cursorTextWidth - GetContentSize().w;
    }
    else if(cursorTextWidth - textOffset < 0)
    {
      textOffset = cursorTextWidth;
    }
    else if(textWidth >= GetContentSize().w && textWidth - textOffset - GetContentSize().w < 0)
    {
      textOffset = textWidth - GetContentSize().w;
    }
    else if(textWidth <= GetContentSize().w)
    {
      textOffset = 0;
    }
  }

  void TextBox::MoveCursor(int delta)
  {
    cursorPos += delta;
    Math::Clamp<int>(&cursorPos, 0, text->GetText().length());
    RecenterText();
  }

  void TextBox::MoveCursorWord(bool forward)
  {
    int delta = forward ? 1 : -1;
    // Offset the cursor to make it easier to use with delta

    if(!forward)
      cursorPos+=delta;
    StringUtils::CharType type = StringUtils::GetCharType(text->GetText()[cursorPos]);
    while(type == StringUtils::GetCharType(text->GetText()[cursorPos]) && cursorPos >= 0 && cursorPos < text->GetText().length())
    {
      cursorPos += delta;
    }
    Math::Clamp<int>(&cursorPos, 0, text->GetText().length());
    if(type != StringUtils::GetCharType(text->GetText()[cursorPos]) && !forward)
      cursorPos -= delta;

    // If it was a whitespace character we keep going
    if(type == StringUtils::GetCharType(' '))
      MoveCursorWord(forward);
    RecenterText();
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
