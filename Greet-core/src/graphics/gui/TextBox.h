#pragma once

#include <graphics/gui/Component.h>
#include <graphics/gui/Label.h>

namespace Greet
{
  class TextBox : public Component
  {
    protected:
      Label text;
      int cursorPos;
      int selectionPos;
      float cursorBlinkTimer;
      bool ctrlDown;
      bool shiftDown;

      bool password;
      std::string str;

    public:
      TextBox(const XMLObject& object, Component* parent);

      void Render(GUIRenderer* renderer) const override;
      void Update(float timeElapsed) override;

      const std::string& GetText() const;
      void SetText(const std::string& text);

      //virtual void MousePressed(const MousePressedEvent& event, const Vec2& translatedPos);
      // virtual void MouseReleased(const MouseReleasedEvent& event, const Vec2& translatedPos);
      //  virtual void MouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos);
      virtual void KeyPressed(const KeyPressedEvent& event) override;
      virtual void KeyReleased(const KeyReleasedEvent& event) override;
      virtual void KeyTyped(const KeyTypedEvent& event) override;

    protected:
      void MoveCursor(int delta);
      void MoveCursorWord(bool forward);
      int GetCursorPos() const;
      int GetSelectionPos() const;
  };

}
