#pragma once

#include <utils/Utils.h>
#include <math/MathFunc.h>
#include <internal/GreetTypes.h>
#include <string>
#include <map>
#include <queue>
#include <event/InputControl.h>

namespace Greet
{
  class InputController
  {
    public:
      friend class Window;
      friend class App;
      friend class Joystick;
    private:
      static std::deque<InputControl*> changed;
      static std::map<ushort, InputWeightControl> inputWeightControls;
    public:
      static void AddKeyButton(ushort keyCode, InputControl* control, float weight, bool overrideValue, uint index);
      static void AddMouseButton(ushort mouseCode, InputControl* control, float weight, bool overrideValue, uint index);
      static void AddMouseMotion(ushort mouseCode, InputControl* control, float weight, bool overrideValue, uint index);
      static void AddMouseWheel(ushort mouseCode, InputControl* control, float weight, bool overrideValue, uint index);
      static void AddGamepadButton(ushort gamepadCode, InputControl* control, float weight, bool overrideValue, uint index);
      static void AddGamepadAxis(ushort gamepadCode, InputControl* control, float weight, bool overrideValue, uint index);

    private:
      static void AddInput(ushort code, InputControl* control, float weight, bool overrideValue, uint index);
      static void KeyButton(ushort keyCode, float value);
      static void MouseButton(ushort mouseCode, float value);
      static void MouseMotion(ushort mouseCode, float value);
      static void MouseWheel(ushort mouseCode, float value);
      static void GamepadButton(ushort gamepadCode, float value);
      static void GamepadAxis(ushort gamepadCode, float value);
      static void UpdateInputControl(ushort code, float value);
      static void FlushInputs();
  };
}
