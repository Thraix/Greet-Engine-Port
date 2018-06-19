#include "InputController.h"

#include <graphics/RenderEngine.h>

#define GAMEPAD_BUTTON_BITS 0x0000 
#define GAMEPAD_AXIS_BITS 0x2000 
#define KEYBOARD_BUTTON_BITS 0x4000
#define MOUSE_BUTTON_BITS 0x6000
#define MOUSE_WHEEL_BITS 0x8000
#define MOUSE_MOTION_BITS 0xa000

#define GAMEPAD_BUTTON(x) ((x & CODE_BITS) | GAMEPAD_BUTTON_BITS)
#define GAMEPAD_AXIS(x) ((x & CODE_BITS) | GAMEPAD_AXIS_BITS)
#define KEYBOARD_BUTTON(x) ((x & CODE_BITS) | KEYBOARD_BUTTON_BITS)
#define MOUSE_BUTTON(x) ((x & CODE_BITS) | MOUSE_BUTTON_BITS)
#define MOUSE_WHEEL(x) ((x & CODE_BITS) | MOUSE_WHEEL_BITS)
#define MOUSE_MOTION(x) ((x & CODE_BITS) | MOUSE_MOTION_BITS)

// Three most significant bytes
#define TYPE_BITS 0xe000

// 13 least significant bytes
#define CODE_BITS 0x1fff

namespace Greet
{

  std::map<ushort, InputWeightControl> InputController::inputWeightControls;
  std::deque<InputControl*> InputController::changed;

  void InputController::AddKeyButton(ushort keyCode, InputControl* control, float weight, bool overrideValue, uint index)
  {
    AddInput(KEYBOARD_BUTTON(keyCode), control,weight, overrideValue,index);
  }

  void InputController::AddMouseButton(ushort mouseCode, InputControl* control, float weight, bool overrideValue, uint index)
  {
    AddInput(MOUSE_BUTTON(mouseCode), control,weight, overrideValue,index);
  }

  void InputController::AddMouseWheel(ushort mouseCode, InputControl* control, float weight, bool overrideValue, uint index)
  {
    AddInput(MOUSE_WHEEL(mouseCode), control,weight, overrideValue,index);
  }

  void InputController::AddMouseMotion(ushort mouseCode, InputControl* control, float weight, bool overrideValue, uint index)
  {
    AddInput(MOUSE_MOTION(mouseCode), control,weight, overrideValue,index);
  }

  void InputController::AddGamepadButton(ushort gamepadCode, InputControl* control, float weight, bool overrideValue, uint index)
  {
    AddInput(GAMEPAD_BUTTON(gamepadCode), control,weight, overrideValue,index);
  }

  void InputController::AddGamepadAxis(ushort gamepadCode, InputControl* control, float weight, bool overrideValue, uint index)
  {
    AddInput(GAMEPAD_AXIS(gamepadCode), control,weight, overrideValue,index);
  }

  void InputController::AddInput(ushort code, InputControl* control, float weight, bool overrideValue, uint index)
  {
    inputWeightControls.emplace(code,InputWeightControl(control,weight,overrideValue,index));
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
    Log::Info(gamepadCode,", ",value);
  }

  void InputController::GamepadAxis(ushort gamepadCode, float value)
  {
    UpdateInputControl(gamepadCode | GAMEPAD_AXIS_BITS,value);
  }

  void InputController::UpdateInputControl(ushort code, float value)
  {
    auto it = inputWeightControls.find(code);
    if(it == inputWeightControls.end())
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
      RenderEngine::InputChanged(*it);
      (*it)->isDirty = false;
      ++it;
      changed.pop_front();
    }

  }
}
