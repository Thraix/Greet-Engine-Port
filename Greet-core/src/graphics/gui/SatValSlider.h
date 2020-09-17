#pragma once

#include <graphics/gui/Component.h>

namespace Greet
{
  class SatValSlider : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(SatValSlider);
    public:
      typedef std::function<void(Component*, float oldValue, float newValue)> OnValueChangeCallback;
    protected:
      Color color;

      OnValueChangeCallback onSatChangeCallback;
      OnValueChangeCallback onValChangeCallback;

    public:
      SatValSlider(const std::string& xmlObject, Component* parent, const std::string& componentType = "SatValSlider");
      SatValSlider(const XMLObject& xmlObject, Component* parent);

      void PreRender(GUIRenderer* renderer, const Vec2f& translation) const override;
      void Render(GUIRenderer* renderer) const override;

      virtual void OnEvent(Event& event, const Vec2f& componentPos) override;

      // Callbacks
      virtual void SetOnSatChangeCallback(OnValueChangeCallback callback);
      virtual void SetOnValChangeCallback(OnValueChangeCallback callback);

      void SetHue(float hue);
      void SetSat(float sat, bool callback = true);
      void SetVal(float val, bool callback = true);

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
