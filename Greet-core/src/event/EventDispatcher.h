#pragma once

#include <map>
#include <vector>
#include <event/Event.h>

namespace Greet {
  class EventDispatcher
  {
    public:
      static void OnEvent(Event&& e);
  };
}
