#pragma once

#include <graphics/gui/Component.h>

namespace Greet
{
  class Slider : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(Slider);
    public:
      typedef std::function<void(Slider*, float oldValue, float newValue)> OnValueChangeCallback;
    protected:

      // The slider indicator component can be any component really
      Component* sliderComponent;
      float value;
      bool vertical = false;
      bool clampSlider = true;
      bool snapSlider = false;
      float minValue;
      float maxValue;
      float stepSize;

      float minPos;
      float maxPos;

      OnValueChangeCallback onValueChangeCallback;



    public:
      Slider(const std::string& name, Component* parent);
      Slider(const XMLObject& xmlObject, Component* parent);

      virtual void Measure(const Vec2& emptyParentSpace, const Vec2& percentageFill) override;
      virtual void OnMeasured() override;

      virtual void Render(GUIRenderer* renderer) const override;

      virtual void OnEvent(Event& event, const Vec2& componentPos) override;

      // Callbacks
      virtual void SetOnValueChangeCallback(OnValueChangeCallback callback);

      Slider& SetVertical(bool vertical);

      virtual void LoadFrameStyle(const MetaFile& metaFile) override;

      Component* GetSliderComponent();

      float GetValue() const;
      float GetMinValue() const;
      float GetMaxValue() const;
      void SetValue(float value);
      float GetSnappedSlider(float sliderValue) const;
      float GetSliderValueFromPos(float pos) const;
      float GetSliderPosFromValue(float value) const;
    protected:
      float GetSliderDirectionSize() const;
      virtual void CallOnValueChangeCallback(float oldValue, float newValue);
  };
}
