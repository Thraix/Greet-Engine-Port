#pragma once

#include <graphics/gui/Text.h>
#include <graphics/gui/Component.h>
#include <graphics/fonts/Font.h>
#include <graphics/fonts/FontManager.h>

namespace Greet {
  class Label : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(Label);
    private:
      Text text;
      bool hasMaxWidth;
      float maxWidth;

    public:
      Label(const std::string& name, Component* parent);
      Label(const std::string& name, Component* parent, const std::string& text, const std::string& fontName, float fontSize);
      Label(const XMLObject& object, Component* parent);
      void Render(GUIRenderer* renderer) const override;

      Label& SetText(const std::string& text);
      const std::string& GetText() const;

      virtual Vec2 GetWrapSize() const override;
      const Vec4& GetColor() const;
      const Font& GetFont() const;
      float GetFontSize() const;

      Label& SetGravity(Text::Gravity grav);
      Label& SetAlign(Text::Align align);
      Label& SetFont(const std::string& font);
      Label& SetFontSize(float fontSize);
      Label& SetColor(const Vec4& color);

  };
}
