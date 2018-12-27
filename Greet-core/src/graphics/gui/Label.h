#pragma once

#include <graphics/gui/Component.h>
#include <graphics/fonts/Font.h>
#include <graphics/fonts/FontManager.h>

namespace Greet {
  class Label : public Component 
  {
    enum class Gravity
    {
      TOP, CENTER, BOTTOM
    };
    private:
      Gravity gravity;
      bool hasMaxWidth;
      float maxWidth;
      std::string str;
      float fontSize;
      Font* font;
      Vec4 color;

    public:
      Label(const XMLObject& object, Component* parent);
      void Render(GUIRenderer* renderer) const override;

      void SetText(const std::string& text);
      const std::string& GetText() const;

      virtual Vec2 GetWrapSize() const override;

      const Vec4& GetColor() const;

      const Font* GetFont() const;

  };
}
