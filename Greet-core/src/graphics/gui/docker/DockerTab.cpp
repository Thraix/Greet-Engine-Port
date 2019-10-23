#include "DockerTab.h"

#include <graphics/gui/Docker.h>

namespace Greet
{
  DockerTab::DockerTab(const XMLObject& object, Docker* docker)
    : DockerInterface{docker}, component{nullptr}
  {
    if(object.GetObjectCount() == 0)
      Log::Warning("DockerTab contains no component");
    else
    {
      component = ComponentFactory::GetComponent(object.GetObject(0), docker);
      if(object.GetObjectCount() >= 2)
        Log::Warning("DockerTab contains more than one component. Consider putting them in a Container");
    }
  }

  DockerTab::~DockerTab()
  {
    if(component)
      delete component;
  }

  void DockerTab::Render(GUIRenderer* renderer) const
  {
    component->PreRender(renderer, docker->GetPosition());
    component->Render(renderer);
    component->PostRender(renderer);
  }

  void DockerTab::SetPosition(const Vec2& _position)
  {
    position = _position;
    if(component)
      component->SetPosition(position);
  }

  void DockerTab::SetSize(const Vec2& _size) 
  {
    size = _size;
    if(component)
    {
      component->Measure();
      component->MeasureFill(size, {1, 1});
    }
  }
}
