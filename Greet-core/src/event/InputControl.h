#pragma once

#include <internal/GreetTypes.h>
#include <math/Vec4.h>
#include <math/Vec3.h>
#include <math/Vec2.h>
#include <string>

namespace Greet
{

  class InputControl
  {
    private:
      friend class InputController;
      std::string name;
      uint size;
      bool isDirty;
      float* values;
      float* deltas;

    public:
      InputControl(const std::string& name, uint size);
      virtual ~InputControl();

    public:
      float GetValue(uint index) const;
      const Vec2& GetValueAsVec2() const;
      const Vec3& GetValueAsVec3() const;
      const Vec4& GetValueAsVec4() const;
      float GetDelta(uint index) const;
      const Vec2& GetDeltaAsVec2() const;
      const Vec3& GetDeltaAsVec3() const;
      const Vec4& GetDeltaAsVec4() const;
      uint GetSize() const {return size;};
      const std::string& GetName() const {return name;}
    private:
      void AddValue(uint index, float value);
      void SetValue(uint index, float value);
  };

  class InputWeightControl
  {
    friend class InputController;
    InputControl* control;
    float weight;
    bool overrideValue;
    uint index;
    InputWeightControl(InputControl* control, float weight, bool overrideValue, uint index)
      : control(control), weight(weight), overrideValue(overrideValue), index(index){}
  };
}
