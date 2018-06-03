#pragma once

#include <graphics/gui/Content.h>
#include <graphics/gui/Label.h>

namespace Greet {
  class Button : public Content
  {
    public:
      Label* label;
      Vec2 size;

    public:
      Button(const XMLObject& object);
      Button(const Vec2& size, const std::string& text);
      Button(const Vec2& size, Content* content);
      virtual ~Button();
      void Render(GUIRenderer* renderer, const Vec2& position) const override;

      float GetWidth() const override;
      float GetHeight() const override;

      // Listeners
      bool IsFocusable() const override;
  };
}
