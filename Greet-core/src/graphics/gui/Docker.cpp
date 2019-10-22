#include "Docker.h"

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(Docker);

  Docker::Docker(const XMLObject& object, Component* parent)
    : Component(object, parent), split{nullptr}
  {
    for(auto&& child : object.GetObjects())
    {
      if(child.GetName() == "DockerSplit")
      {
        if(split != nullptr)
          split = new DockerSplit(child, this);
        else
          Log::Error("NOTIMPLEMENTED: Docker currently only supports one DockerSplit");
      }
      else if(child.GetName() == "DockerContainer") 
      {
        Log::Error("NOTIMPLEMENTED: Docker containing only DockerContainer is currently not supported");
        // This should contain DockerTabs which are actual tabs
        // Maybe this should be wrapped in a DockerSplit
        // TODO: Add docker container component 
      }
      else if(child.GetName() == "DockerTab")
      {
        Log::Error("NOTIMPLEMENTED: Docker containing only DockerTab is currently not supported");
        // Add Docker tab component 
        // TODO: Maybe this should be wrapped in a docker container
      }
      else if(child.GetName() == "TabLayout")
      {
        Log::Error("NOTIMPLEMENTED: TabLayout in Docker");
        // This will define how the tab button will look like in the top of the DockerContainer
        // TODO: Set tab style
      }
      else
      {
        Log::Error("Unknown component in Docker. Component=", child.GetName());
      }
    }
  }

  void Docker::Render(GUIRenderer* renderer) const
  {

  }
}
