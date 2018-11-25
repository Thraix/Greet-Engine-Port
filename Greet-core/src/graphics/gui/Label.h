#pragma once

#include <graphics/gui/Component.h>
#include <graphics/fonts/Font.h>
#include <graphics/fonts/FontManager.h>

namespace Greet {
  class Label : public Component 
  {
    private:
      bool hasMaxWidth;
      float maxWidth;
      std::string str;
      float fontSize;
      Font* font;
      Vec4 color;

    public:
      Label();
      Label(const XMLObject& object, Component* parent);
      void Render(GUIRenderer* renderer) const override;


      void SetText(const std::string& text);
      const std::string& GetText() const;

      virtual float CalculateWidth() const override;
      virtual float CalculateHeight() const override;

      const Vec4& GetColor() const;

      const Font* GetFont() const;

  };
}
