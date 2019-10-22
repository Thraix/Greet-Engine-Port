#include "DockerTab.h"

#include <graphics/gui/Docker.h>

namespace Greet
{
  DockerTab::DockerTab(const XMLObject& object, Docker* parent)
    : component{nullptr}
  {
    if(object.GetObjectCount() == 0)
      Log::Warning("DockerTab contains no component");
    else
    {
      component = ComponentFactory::GetComponent(object.GetObject(0), parent);
      if(object.GetObjectCount() >= 2)
        Log::Warning("DockerTab contains more than one component. Consider putting them in a Container");
    }
  }

  DockerTab::~DockerTab()
  {
    if(component)
      delete component;
  }
}
