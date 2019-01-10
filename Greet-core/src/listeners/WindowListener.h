#pragma once

#include <listeners/WindowFocus.h>
#include <listeners/WindowResize.h>

namespace Greet {
  class WindowListener : public WindowFocusListener, public WindowResizeListener
  {

  };
}
