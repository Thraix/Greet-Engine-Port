#pragma once

#include <graphics/gui/Component.h>

namespace Greet
{
  class Slider : public Component
  {
    private:
      static uint SLIDER_FLAG_FORCE_INSIDE;
      static uint SLIDER_FLAG_SNAP;
      // The slider indicator component can be any component really
      Component* sliderComponent;
      bool mouseHeld;
      float sliderPos;
      uint flags;
      float minValue;
      float maxValue;
      float stepSize;

    public:
      Slider(const XMLObject& xmlObject, Component* parent);

      virtual void Render(GUIRenderer* renderer) const override;

      virtual void MousePressed(const MousePressedEvent& event, const Vec2& translatedPos) override;
      virtual void MouseReleased(const MouseReleasedEvent& event, const Vec2& translatedPos) override;
      virtual void MouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos) override;

      float GetSnappedSlider(float sliderValue);
      float GetSliderValueFromPos(float pos);
      float GetSliderPosFromValue(float value);
  };
}
