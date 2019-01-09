#pragma once

#include <graphics/gui/Component.h>

namespace Greet
{
  class SatValSlider : public Component 
  {
    public:
      typedef std::function<void(Component*, float oldValue, float newValue)> OnSatChangeCallback;
      typedef std::function<void(Component*, float oldValue, float newValue)> OnValChangeCallback;
    protected:
      float sat;
      float val;

      Component* sliderComponent;
      OnSatChangeCallback onSatChangeCallback;
      OnValChangeCallback onValChangeCallback;

    public:
      SatValSlider(const XMLObject& xmlObject, Component* parent);

      void PreRender(GUIRenderer* renderer, const Vec2& translation) const override;
      void Render(GUIRenderer* renderer) const override;

      virtual void MousePressed(const MousePressedEvent& event, const Vec2& translatedPos) override;
      virtual void MouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos) override;

      // Callbacks
      virtual void SetOnSatChangeCallback(OnSatChangeCallback callback);
      virtual void SetOnValChangeCallback(OnValChangeCallback callback);

      void SetSat(float sat);
      void SetVal(float val);

      float GetSat() const;
      float GetVal() const;

      float GetSliderSatFromPos(float pos) const;
      float GetSliderPosFromSat(float value) const;

      float GetSliderValFromPos(float pos) const;
      float GetSliderPosFromVal(float value) const;

    protected:
      virtual void CallOnSatChangeCallback(float oldSat, float newSat);
      virtual void CallOnValChangeCallback(float oldVal, float newVal);

  };
}