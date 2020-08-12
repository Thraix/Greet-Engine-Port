#pragma once

#include <graphics/fonts/Font.h>
#include <graphics/fonts/FontManager.h>
#include <graphics/renderers/GUIRenderer.h>
#include <math/Vec4.h>

namespace Greet
{
  class Text
  {
    public:
      enum class Gravity
      {
        Top, Center, Bottom
      };

      enum class Align
      {
        Left, Center, Right
      };

      enum class OverlapMode
      {
        Dots, Wrap, Ignore
      };

      std::string str;
      Font font;
      float fontSize;
      Vec4 color = {0, 0, 0, 1};
      Gravity gravity;
      Align align;
      OverlapMode overlapMode;

      Text(const std::string& str, const std::string& fontname, float fontSize, const Vec4& color = {0, 0, 0, 1}, Gravity gravity = Gravity::Center, Align align = Align::Left, OverlapMode overlapMode = OverlapMode::Dots);

      void Render(GUIRenderer* renderer, const Vec2& contentSize) const;

      void SetFont(const std::string& fontname)
      {
        font = FontManager::Get(fontname, fontSize);
      }

      std::vector<std::string> GetStringLines(float maxWidth) const;
      float GetWrapWidth() const;
      float GetWrapHeight(float maxWidth) const;

      void SetFontSize(float fontSize)
      {
        font = FontManager::Get(font.GetName(), fontSize);
      }
  };
}
