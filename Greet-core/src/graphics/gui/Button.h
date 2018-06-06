#pragma once

#include <graphics/gui/Content.h>
#include <graphics/gui/Label.h>

namespace Greet {
  class Button : public Content
  {
    protected:
      float fontSize;
      Vec4 textColor;
    public:
      Button(const XMLObject& object, Content* parent);
      virtual ~Button();
      void Render(GUIRenderer* renderer, const Vec2& position) const override;

      // Listeners
      bool IsFocusable() const override;
  };
}
