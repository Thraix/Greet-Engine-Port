#pragma once

namespace Greet {
  class WindowFocusListener
  {
    public:
      virtual void WindowFocus(bool focus) = 0;
  };
}
