#include "Docker.h"

#include <input/InputDefines.h>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(Docker);

  Docker::Docker(const XMLObject& object, Component* parent)
    : Component(object, parent), split{nullptr}, dockerTab{nullptr}
  {
    m_isFocusable = true;
    for(auto&& child : object.GetObjects())
    {
      if(child.GetName() == "DockerSplit")
      {
        if(split == nullptr)
          split = new DockerSplit(child, this, nullptr);
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
      split = new DockerSplit({"DockerSplit",{},""}, this, nullptr);
    }
  }

  void Docker::GrabDockerTab(DockerTab* tab)
  {
    dockerTab = tab;
  }

  void Docker::HandleDroppedTab(MouseReleaseEvent& event, const Vec2& componentPos)
  {
    Vec2 mousePos = event.GetPosition() - componentPos;
    DockerContainer* container = dockerTab->GetContainer();
    int tabIndex = container->GetTabIndex(dockerTab);

    int splitLimit = 0;
    bool globalSplit = false;
    bool vertical = false;
    bool front = true;

    if(mousePos.x < splitLimit)
    {
      container->RemoveTab(tabIndex);
      vertical = false;
      globalSplit = true;
      front = true;
    }
    else if(mousePos.x >= GetSize().w - splitLimit)
    {
      container->RemoveTab(tabIndex);
      vertical = false;
      globalSplit = true;
      front = false;
    }
    else if(mousePos.y < splitLimit)
    {
      container->RemoveTab(tabIndex);
      vertical = true;
      globalSplit = true;
      front = true;
    }
    else if(mousePos.y >= GetSize().h - splitLimit)
    {
      container->RemoveTab(tabIndex);
      vertical = true;
      globalSplit = true;
      front = false;
    }

    if(globalSplit)
    {
      DockerContainer* container = new DockerContainer(dockerTab, this, nullptr);
      if(split->IsVertical() != vertical)
      {
        split = new DockerSplit(split, this, nullptr, vertical);
        OnMeasured();
      }
      split->AddContainer(front ? 0 : split->GetDockerCount(), container);
      dockerTab = nullptr;
      return;
    }

    if(IsMouseInside(mousePos))
    {
      if(split->HandleDroppedTab(dockerTab, event, componentPos))
      {
        container->RemoveTab(tabIndex);
      }
    }
    dockerTab = nullptr;

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
    if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
    {
      MouseReleaseEvent& e = static_cast<MouseReleaseEvent&>(event);
      if(dockerTab != nullptr)
      {
        if(e.GetButton() == GREET_MOUSE_1)
        {
          HandleDroppedTab(e, componentPos);
        }
        return;
      }
      split->OnEvent(event, componentPos);
    }
    split->OnEvent(event, componentPos);
  }

  void Docker::OnMeasured()
  {
    split->SetSize(GetSize());
    // Update positions of child dockers
    split->SetPosition({0,0});
  }

  Component* Docker::GetComponentByNameNoCast(const std::string& name)
  {
    Component* comp = Component::GetComponentByNameNoCast(name);
    if(comp)
      return comp;
    return split->GetComponentByNameNoCast(name);
  }

  void Docker::SetGUIScene(GUIScene* scene)
  {
    guiScene = scene;
    split->SetGUIScene(scene);
  }
}
