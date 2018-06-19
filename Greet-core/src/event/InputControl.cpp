#include "InputControl.h"

#include <math/MathFunc.h>

namespace Greet
{

  InputControl::InputControl(const std::string& name, uint size)
    : name(name), size(size), isDirty(false), values(new float[size]), deltas(new float[size])
  {

  }

  InputControl::~InputControl() {
    delete[] values;
    delete[] deltas;
  }

  float InputControl::GetValue(uint index) const 
  {
#if defined(_DEBUG_INPUT) ||  defined(_DEBUG_ALL)
    if(index >= size)
    {
      Log::Warning("Input index out of bounds.");
      return 0.0f;
    }
#endif
    return values[index];
  }

  const Vec2& InputControl::GetValueAsVec2() const
  {
#if defined(_DEBUG_INPUT) ||  defined(_DEBUG_ALL)
    if(size != 2)
    {
      Log::Error("Input size is not 2.");
    }
#endif
    return *(Vec2*)values;
  }
  const Vec3& InputControl::GetValueAsVec3() const
  {
#if defined(_DEBUG_INPUT) ||  defined(_DEBUG_ALL)
    if(size != 3)
    {
      Log::Error("Input size is not 3.");
    }
#endif
    return *(Vec3*)values;
  }
  const Vec4& InputControl::GetValueAsVec4() const
  {
#if defined(_DEBUG_INPUT) ||  defined(_DEBUG_ALL)
    if(size != 4)
    {
      Log::Error("Input size is less than 4.");
    }
#endif
    return *(Vec4*)values;
  }
  float InputControl::GetDelta(uint index) const 
  {
#if defined(_DEBUG_INPUT) ||  defined(_DEBUG_ALL)
    if(index >= size)
    {
      Log::Error("Input index out of bounds.");
      return 0.0f;
    }
#endif
    return deltas[index];
  }
  const Vec2& InputControl::GetDeltaAsVec2() const
  {
#if defined(_DEBUG_INPUT) ||  defined(_DEBUG_ALL)
    if(size != 2)
    {
      Log::Error("Input size is not 2.");
    }
#endif
    return *(Vec2*)deltas;
  }
  const Vec3& InputControl::GetDeltaAsVec3() const
  {
#if defined(_DEBUG_INPUT) ||  defined(_DEBUG_ALL)
    if(size != 3)
    {
      Log::Error("Input size is not 3.");
    }
#endif
    return *(Vec3*)deltas;
  }
  const Vec4& InputControl::GetDeltaAsVec4() const
  {
#if defined(_DEBUG_INPUT) ||  defined(_DEBUG_ALL)
    if(size != 4)
    {
      Log::Error("Input size is not 4.");
    }
#endif
    return *(Vec4*)deltas;
  }

  void InputControl::AddValue(uint index, float value) 
  {
#if defined(_DEBUG_INPUT) ||  defined(_DEBUG_ALL)
    if(index >= size)
    {
      Log::Warning("Input index out of bounds.");
      return;
    }
#endif
    if(value != 0)
    {
      deltas[index]-=values[index];
      values[index]+=value;
      //Greet::Math::Clamp(&values[index], -1,1);
      deltas[index]+=values[index];
      isDirty = true;
    }
  }

  void InputControl::SetValue(uint index, float value)
  {
#if defined(_DEBUG_INPUT) ||  defined(_DEBUG_ALL)
    if(index >= size)
    {
      Log::Warning("Input index out of bounds.");
      return;
    }
#endif
    if(value != values[index])
    {
      deltas[index]= values[index];
      values[index] = value;
      //Greet::Math::Clamp(&values[index], -1,1);
      deltas[index] = values[index] - deltas[index];
      isDirty = true;
    }
    else
    {
      deltas[index] = 0;
    }
  }
}
