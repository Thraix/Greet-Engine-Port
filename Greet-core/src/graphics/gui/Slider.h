#pragma once

#include <graphics/gui/Component.h>

namespace Greet
{
  class Slider : public Component
  {
    private:
      static uint SLIDER_FLAG_FORCE_INSIDE;
      static uint SLIDER_FLAG_SNAP;
      static uint SLIDER_FLAG_VERTICAL;

      // The slider indicator component can be any component really
      Component* sliderComponent;
      bool mouseHeld;
      float sliderPos;
      uint flags;
      float minValue;
      float maxValue;
      float stepSize;

      float minPos;
      float maxPos;

    public:
      Slider(const XMLObject& xmlObject, Component* parent);

      virtual void Render(GUIRenderer* renderer) const override;

      virtual void MousePressed(const MousePressedEvent& event, const Vec2& translatedPos) override;
      virtual void MouseReleased(const MouseReleasedEvent& event, const Vec2& translatedPos) override;
      virtual void MouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos) override;
      virtual void Resized() override;

      float GetValue() const;
      void SetValue(float value);
      float GetSnappedSlider(float sliderValue) const;
      float GetSliderValueFromPos(float pos) const;
      float GetSliderPosFromValue(float value) const;
    private:
      float GetSliderDirectionSize() const;
  };
}
