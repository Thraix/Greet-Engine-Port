#include "EventDispatcher.h"

#include <functional>
#include <utils/Utils.h>
#include <logging/Logger.h>
#include <graphics/RenderEngine.h>

namespace Greet {

  void EventDispatcher::OnEvent(Event&& event)
  {
    RenderEngine::OnEvent(event);
  }
}
