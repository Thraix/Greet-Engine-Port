#pragma  once

#include <event/InputControl.h>

namespace Greet
{
  // If Request focus is called no other listeners will be notified on input changes.
  enum InputControlRequest 
  {
    NOTHING, FOCUS, UNFOCUS, DONE
  };

  class InputControlListener
  {
    virtual InputControlRequest OnInputChanged(const InputControl* control) = 0;
  };
}
