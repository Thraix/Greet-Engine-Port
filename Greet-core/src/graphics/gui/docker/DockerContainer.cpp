#include "DockerContainer.h"

#include <graphics/fonts/FontManager.h>
#include <graphics/gui/Docker.h>
#include <input/InputDefines.h>

namespace Greet
{
  DockerContainer::DockerContainer(const XMLObject& object, Docker* docker)
    : DockerInterface{docker}
  {
    for(auto&& objectChild : object.GetObjects())
    {
      if(objectChild.GetName() == "DockerTab")
      {
        children.push_back(new DockerTab(objectChild, docker));
      }
      else
      {
        Log::Error("Unknown component in DockerContainer. Component=", objectChild.GetName());
      }
    }
  }

  DockerContainer::~DockerContainer()
  {
    for(auto&& child : children)
      delete child;
  }

  void DockerContainer::Render(GUIRenderer* renderer) const
  {
    children[currentTab]->Render(renderer);

    renderer->PushTranslation(docker->GetPosition() + position);
    // 263238
    renderer->SubmitRect({0.0f, 0.0f}, {size.w, TAB_HEIGHT}, {0.039,0.078,0.09,1}, false);
    for(int i = 0;i<children.size();i++)
    {
      Vec4 color = {0.5f,0.5f,0.5f,1.0f};
      if(currentTab == i)
        color = {1,1,1,1};
      else if(hover && hoverTab == i)
        color = {0.8,0.8,0.8,1};

      renderer->SubmitRect({(float)i * (TAB_WIDTH + 1), 0.0f}, {TAB_WIDTH, TAB_HEIGHT}, color, false);
      renderer->SubmitString(children[i]->GetTitle(), {(float)i * (TAB_WIDTH + 1) + TAB_PADDING, 12}, FontManager::Get("noto", 12), {0,0,0,1}, false);
    }
    renderer->PopTranslation();
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

    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      if(e.GetButton() == GREET_MOUSE_1)
      {
        Vec2 pos = e.GetPosition() - componentPos;
        int tab = GetTab(pos);
        if(tab >= 0)
        {
          currentTab = tab;
          return;
        }
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
      Vec2 pos = e.GetPosition() - componentPos;
      int tab = GetTab(pos);
      if(tab >= 0)
      {
        hoverTab = tab;
        hover = true;
        return;
      }
      else
      {
        hover = false;
      }
    }
    children[currentTab]->OnEvent(event, componentPos);
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

  int DockerContainer::GetTab(const Vec2& mousePos)
  {
    if(mousePos.y >= 0 && mousePos.y < TAB_HEIGHT)
    {
      int tab = floor(mousePos.x / (TAB_WIDTH + 1));
      if(tab >= 0 && tab < children.size())
      {
        return tab;
      }
    }
    return -1;

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
      child->SetPosition(_position);
    }
  }

  void DockerContainer::SetSize(const Vec2& _size)
  {
    size = _size;
    for(auto&& child : children)
    {
      child->SetSize(_size);
    }
  }
}
