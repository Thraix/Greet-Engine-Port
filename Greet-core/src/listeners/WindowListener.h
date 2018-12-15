#pragma once

#include "WindowFocus.h"
#include "WindowResize.h"

namespace Greet {
  class WindowListener : public WindowFocusListener, public WindowResizeListener
  {

  };
}
