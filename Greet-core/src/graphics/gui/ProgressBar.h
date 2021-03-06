#pragma once

#include <graphics/gui/Component.h>

namespace Greet
{
  class ProgressBar : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(ProgressBar);
    protected:
      float value;
      float* valueReference;
      float min;
      float max;

      //////////////////
      // Style variables
      bool vertical = false;
      bool reverse = false;
      Color progressColor{1,1,1,1};

    public:
      ProgressBar(const XMLObject& object, Component* parent);

      virtual void Render(GUIRenderer* renderer) const override;
      virtual void Update(float timeElapsed) override;

      void AttachValueReference(float* reference);

      float GetValue() const;
      float* GetValueReference() const;
      void SetValue(float value);
  };
}
