#pragma once

#include <graphics/gui/Component.h>

namespace Greet
{
  // This class will ignore parent padding?
  class Divider : public Component
  {
    protected:
      bool vertical;
    public:
      Divider(const XMLObject& object, Component* parent);
      void PreRender(GUIRenderer* renderer, const Vec2& translation) const override;

      // Will return a newly calculated value
      virtual float CalculateWidth() const override;
      virtual float CalculateHeight() const override;
  };
}
