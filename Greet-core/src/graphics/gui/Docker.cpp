#include "Docker.h"

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(Docker);

  Docker::Docker(const XMLObject& object, Component* parent)
    : Component(object, parent), split{nullptr}
  {
    m_isFocusable = true;
    for(auto&& child : object.GetObjects())
    {
      if(child.GetName() == "DockerSplit")
      {
        if(split == nullptr)
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
    if(split == nullptr)
    {
      Log::Error("No DockerSplit found in Docker");
      split = new DockerSplit({"DockerSplit",{},""}, this);
    }
  }

  void Docker::Render(GUIRenderer* renderer) const
  {
    split->Render(renderer);
  }

  void Docker::Update(float timeElapsed)
  {
    split->Update(timeElapsed);
  }

  void Docker::OnEvent(Event& event, const Vec2& componentPos) 
  {
    split->OnEvent(event, componentPos);
  }

  void Docker::OnMeasured()
  {
    split->SetSize(GetSize());
    // Update positions of child dockers
    split->SetPosition({0,0});
  }

  void Docker::SetGUIScene(GUIScene* scene)
  {
    guiScene = scene;
    split->SetGUIScene(scene);
  }
}
