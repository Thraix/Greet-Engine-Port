#include "Docker.h"

#include <input/InputDefines.h>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(Docker);

  Docker::Docker(const XMLObject& object, Component* parent)
    : Component(object, parent), split{nullptr}, grabbedTab{nullptr}, tabButton{}
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
      else if(child.GetName() == "TabButton")
      {
        tabButton = child;
      }
      else if(child.GetName() == "SplitIcon")
      {
        splitIconStyle = child;
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
    edgeColor = GUIUtils::GetColorFromXML(object, "edgeColor", currentStyle->backgroundColor);
    edgeWidth = GUIUtils::GetIntFromXML(object, "edgeSize", 10);
    edgeBorderSize  = GUIUtils::GetIntFromXML(object, "edgeBorder", 0);
    edgeBorderColor = GUIUtils::GetColorFromXML(object, "edgeBorderColor", Vec4{1,1,1,1});
  }

  void Docker::GrabDockerTab(DockerTab* tab)
  {
    grabbedDistance = 0;
    grabbedTab = tab;
  }

  const XMLObject& Docker::GetTabButton() const
  {
    return tabButton;
  }

  const XMLObject& Docker::GetSplitIconStyle() const
  {
    return splitIconStyle;
  }

  void Docker::HandleDroppedTab(MouseReleaseEvent& event, const Vec2& componentPos)
  {
    Vec2 mousePos = event.GetPosition() - componentPos;

    int splitLimit = 0;

    if(mousePos.x < splitLimit)
    {
      AddSplitLeft(grabbedTab);
    }
    else if(mousePos.x >= GetSize().w - splitLimit)
    {
      AddSplitRight(grabbedTab);
    }
    else if(mousePos.y < splitLimit)
    {
      AddSplitAbove(grabbedTab);
    }
    else if(mousePos.y >= GetSize().h - splitLimit)
    {
      AddSplitBelow(grabbedTab);
    }
    else if(IsMouseInside(mousePos))
    {
      split->HandleDroppedTab(grabbedTab, event, componentPos);
    }
    grabbedTab = nullptr;
  }

  void Docker::Render(GUIRenderer* renderer) const
  {
    split->Render(renderer);
  }

  void Docker::Update(float timeElapsed)
  {
    if(childrenChanged)
    {
      split->MergeSimilarSplits();
      /* split->DebugPrint(0); */
      childrenChanged = false;
    }
    split->Update(timeElapsed);
  }

  void Docker::OnEvent(Event& event, const Vec2& componentPos)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
    {
      MouseReleaseEvent& e = static_cast<MouseReleaseEvent&>(event);
      if(IsHoldingTab())
      {
        if(e.GetButton() == GREET_MOUSE_1)
        {
          HandleDroppedTab(e, componentPos);
        }
        return;
      }
      else if(grabbedTab != nullptr)
      {
        grabbedTab = nullptr;
        return;
      }
      split->OnEvent(event, componentPos);
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      if(grabbedTab != nullptr)
      {
        MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
        grabbedDistance += e.GetDeltaPosition().Length();
      }
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

  bool Docker::IsHoldingTab() const
  {
    return grabbedTab != nullptr && grabbedDistance > 20;
  }

  void Docker::MarkDirty()
  {
    childrenChanged = true;
  }

  void Docker::AddSplitLeft(DockerTab* tab)
  {
    Split(tab, false, true);
  }

  void Docker::AddSplitRight(DockerTab* tab)
  {
    Split(tab, false, false);
  }

  void Docker::AddSplitAbove(DockerTab* tab)
  {
    Split(tab, true, true);
  }

  void Docker::AddSplitBelow(DockerTab* tab)
  {
    Split(tab, true, false);
  }

  void Docker::Split(DockerTab* tab, bool vertical, bool before)
  {
    if(tab->GetContainer() != nullptr)
      tab->GetContainer()->RemoveTab(tab);

    if(split->IsVertical() != vertical)
    {
      split = new DockerSplit(split, this, nullptr, vertical);
    }

    DockerContainer* container = new DockerContainer(grabbedTab, this, nullptr);
    split->AddDocker(before ? 0 : split->GetDockerCount(), container);
    OnMeasured();
  }
}
