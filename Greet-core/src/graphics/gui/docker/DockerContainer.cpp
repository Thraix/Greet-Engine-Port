#include "DockerContainer.h"

#include <graphics/fonts/FontManager.h>
#include <graphics/gui/Docker.h>
#include <input/InputDefines.h>

namespace Greet
{
  DockerContainer::DockerContainer(const XMLObject& object, Docker* docker, DockerSplit* parent)
    : DockerInterface{docker, parent}, currentTab{0}
  {
    for(auto&& objectChild : object.GetObjects())
    {
      if(objectChild.GetName() == "DockerTab")
      {
        children.push_back(new DockerTab(objectChild, docker, this));
      }
      else
      {
        Log::Error("Unknown component in DockerContainer. Component=", objectChild.GetName());
      }
    }
    button = new Button(docker->GetTabButton(), docker);
  }

  DockerContainer::DockerContainer(DockerTab* tab, Docker* docker, DockerSplit* parent)
    : DockerInterface{docker, parent}, currentTab{0}
  {
    children.push_back(tab);
    tab->SetContainer(this);
    button = new Button(docker->GetTabButton(), docker);
  }

  DockerContainer::~DockerContainer()
  {
    for(auto&& child : children)
      delete child;
    delete button;
  }

  void DockerContainer::Render(GUIRenderer* renderer) const
  {
    if(currentTab < 0 || currentTab >= children.size())
      return;

    renderer->PushViewport(position, size);
    children[currentTab]->Render(renderer);

    renderer->PushTranslation(docker->GetPosition() + position);
    // #263238
    renderer->SubmitRect({0.0f, 0.0f}, {size.w, button->GetHeight()}, {0.039,0.078,0.09,1}, false);
    for(int i = 0;i<children.size();i++)
    {
      button->PreRender(renderer, {(float)i * ((float)button->GetWidth() + 1), 0.0f});
      button->SetText(children[i]->GetTitle());
      button->Render(renderer);
      button->PostRender(renderer);
    }
    renderer->PopTranslation();
    renderer->PopViewport();
  }

  void DockerContainer::Update(float timeElapsed)
  {
    for(auto&& child : children)
    {
      child->Update(timeElapsed);
    }
  }

  void DockerContainer::OnEvent(Event& event, const Vec2& componentPos)
  {
    if(currentTab < 0 || currentTab >= children.size())
      return;
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      if(e.GetButton() == GREET_MOUSE_1)
      {
        Vec2 pos = e.GetPosition() - componentPos;
        int tab = GetTab(pos);
        if(tab < children.size())
        {
          SelectTab(tab);
          docker->GrabDockerTab(children[tab]);
          return;
        }
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
      Vec2 pos = e.GetPosition() - componentPos;
      int tab = GetTab(pos);
      if(tab < children.size())
      {
        HoverTab(tab);
        return;
      }
      else
      {
        UnhoverTab();
      }
    }
    children[currentTab]->OnEvent(event, componentPos);
  }

  bool DockerContainer::HandleDroppedTab(DockerTab* tab, MouseReleaseEvent& event, const Vec2& componentPos)
  {
    // Only move inside the current container
    if(tab->GetContainer() == this)
    {
      int tabIndex = GetTab(event.GetPosition() - componentPos);
      if(tabIndex < children.size())
      {
        int current = GetTabIndex(tab);
        if(current == tabIndex)
          return false;
        if(current > tabIndex)
          std::rotate(children.begin()+tabIndex, children.begin() + current, children.begin() + current+1);
        else
          std::rotate(children.begin()+current, children.begin() + 1, children.begin() + tabIndex+1);
        UnhoverTab();
        SelectTab(tabIndex);
        return false;
      }
      return false;
    }
    else
    {
      DockerContainer* tabContainer = tab->GetContainer();

      int tabIndex = GetTab(event.GetPosition() - componentPos);
      int tabIndexContainer = tabContainer->GetTabIndex(tab);

      // Fix this container
      children.insert(children.begin() + tabIndex, tab);
      tab->SetPosition(position);
      tab->SetSize(size);
      tab->SetContainer(this);
      SelectTab(tabIndex);

      return true;
    }
  }

  void DockerContainer::SelectTab(int i)
  {
    ASSERT(i >= 0 && i < children.size(), "Index out of bound");
    currentTab = i;
  }

  void DockerContainer::ClampSelectedTab()
  {
    if(currentTab >= children.size())
      currentTab = children.size()-1;
  }

  void DockerContainer::HoverTab(int i)
  {
    ASSERT(i >= 0 && i < children.size(), "Index out of bound");
    hoverTab = i;
    hover = true;
  }

  void DockerContainer::UnhoverTab()
  {
    hover = false;
  }

  void DockerContainer::RemoveTab(int i)
  {
    ASSERT(i >= 0 && i < children.size(), "Index out of bound");
    children.erase(children.begin() + i);

    DockerSplit* split = static_cast<DockerSplit*>(parent);
    if(children.size() == 0)
      split->RemoveDocker(split->GetDockerIndex(this));
    else
      ClampSelectedTab();
  }

  Component* DockerContainer::GetComponentByNameNoCast(const std::string& name)
  {
    for(auto&& child : children)
    {
      Component* comp = child->GetComponentByNameNoCast(name);
      if(comp)
        return comp;
    }
    return nullptr;
  }

  Vec2 DockerContainer::GetMinSize() const
  {
    return Vec2{20, button->GetHeight()+20};
  }

  int DockerContainer::GetTabIndex(DockerTab* tab)
  {
    for(int i = 0;i<children.size(); ++i)
    {
      if(children[i] == tab)
        return i;
    }
    ASSERT(false, "Tab is not inside DockerContainer");
    return children.size();
  }

  int DockerContainer::GetTab(const Vec2& mousePos)
  {
    if(mousePos.y >= 0 && mousePos.y < button->GetHeight())
    {
      int tab = floor(mousePos.x / (button->GetWidth() + 1));
      if(tab < children.size())
      {
        return tab;
      }
    }
    return children.size();
  }

  int DockerContainer::GetTabCount()
  {
    return children.size();
  }

  void DockerContainer::SetGUIScene(GUIScene* scene)
  {
    for(auto&& child : children)
    {
      child->SetGUIScene(scene);
    }
  }

  void DockerContainer::SetPosition(const Vec2& _position)
  {
    position = _position;
    for(auto&& child : children)
    {
      child->SetPosition(_position + Vec2{0.0f, button->GetHeight()});
    }
  }

  void DockerContainer::SetSize(const Vec2& _size)
  {
    size = _size;
    for(auto&& child : children)
    {
      child->SetSize(_size);
    }
    button->Measure();
    button->MeasureFill(size, {1.0f,1.0f});
  }
}
