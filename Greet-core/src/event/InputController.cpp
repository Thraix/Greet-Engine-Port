#include "InputController.h"

#define GAMEPAD_BUTTON_BITS 0x0000 
#define GAMEPAD_AXIS_BITS 0x2000 
#define KEYBOARD_BUTTON_BITS 0x4000
#define MOUSE_BUTTON_BITS 0x6000
#define MOUSE_WHEEL_BITS 0x8000
#define MOUSE_MOTION_BITS 0xa000

// Three most significant bytes
#define TYPE_BITS 0xe000

// 13 most insignificant bytes
#define CODE_BITS 0x1fff

namespace Greet
{

  std::map<ushort, InputWeightControl> InputController::inputControls;
  std::deque<InputControl*> InputController::changed;
  InputControllerTest* InputController::test;

  void InputController::AddKeyButton(ushort keyCode, InputControl* control, float weight, bool overrideValue, uint index)
  {
    keyCode = (keyCode & CODE_BITS) | KEYBOARD_BUTTON_BITS;
    inputControls.emplace(keyCode,InputWeightControl(control, weight, overrideValue, index));
  }

  void InputController::AddMouseButton(ushort mouseCode, InputControl* control, float weight, bool overrideValue, uint index)
  {
    mouseCode = (mouseCode & CODE_BITS) | MOUSE_BUTTON_BITS;
    inputControls.emplace(mouseCode,InputWeightControl(control, weight, overrideValue, index));
  }

  void InputController::AddMouseWheel(ushort mouseCode, InputControl* control, float weight, bool overrideValue, uint index)
  {
    mouseCode = (mouseCode & CODE_BITS) | MOUSE_WHEEL_BITS;
    inputControls.emplace(mouseCode,InputWeightControl(control, weight, overrideValue, index));
  }

  void InputController::AddMouseMotion(ushort mouseCode, InputControl* control, float weight, bool overrideValue, uint index)
  {
    mouseCode = (mouseCode & CODE_BITS) | MOUSE_MOTION_BITS;
    inputControls.emplace(mouseCode,InputWeightControl(control, weight, overrideValue, index));
  }

  void InputController::AddGamepadButton(ushort gamepadCode, InputControl* control, float weight, bool overrideValue, uint index)
  {
    gamepadCode = (gamepadCode & CODE_BITS) | GAMEPAD_BUTTON_BITS;
    inputControls.emplace(gamepadCode,InputWeightControl(control, weight, overrideValue, index));
  }

  void InputController::AddGamepadAxis(ushort gamepadCode, InputControl* control, float weight, bool overrideValue, uint index)
  {
    gamepadCode = (gamepadCode & CODE_BITS) | GAMEPAD_AXIS_BITS;
    inputControls.emplace(gamepadCode,InputWeightControl(control, weight, overrideValue, index));
  }

  void InputController::KeyButton(ushort keyCode, float value)
  {
    UpdateInputControl(keyCode | KEYBOARD_BUTTON_BITS,value);
  }

  void InputController::MouseButton(ushort mouseCode, float value)
  {
    UpdateInputControl(mouseCode | MOUSE_BUTTON_BITS,value);
  }

  void InputController::MouseMotion(ushort mouseCode, float value)
  {
    UpdateInputControl(mouseCode | MOUSE_MOTION_BITS,value);
  }

  void InputController::MouseWheel(ushort mouseCode, float value)
  {
    UpdateInputControl(mouseCode | MOUSE_WHEEL_BITS,value);
  }

  void InputController::GamepadButton(ushort gamepadCode, float value)
  {
    UpdateInputControl(gamepadCode | GAMEPAD_BUTTON_BITS,value);
  }

  void InputController::GamepadAxis(ushort gamepadCode, float value)
  {
    UpdateInputControl(gamepadCode | GAMEPAD_AXIS_BITS,value);
  }

  void InputController::UpdateInputControl(ushort code, float value)
  {
    auto it = inputControls.find(code);
    if(it == inputControls.end())
      return;
    InputControl* control = it->second.control;
    bool wasDirty = control->isDirty;
    if(it->second.overrideValue)
      control->SetValue(it->second.index, value*it->second.weight);
    else
      control->AddValue(it->second.index,value*it->second.weight);
    if(!wasDirty && control->isDirty)
    {
      changed.push_back(control);
    }
  }
  void InputController::FlushInputs()
  {
    auto it = changed.begin();
    while(it != changed.end())
    {
      test->OnInputChanged(*it);
      (*it)->isDirty = false;
      ++it;
      changed.pop_front();
    }

  }
}
