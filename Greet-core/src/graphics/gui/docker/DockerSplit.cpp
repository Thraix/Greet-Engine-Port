#include "DockerSplit.h"

#include <graphics/gui/Docker.h>

namespace Greet
{
  DockerSplit::DockerSplit(const XMLObject& object, Docker* docker)
    : DockerInterface{docker}
  {
    vertical = GUIUtils::GetBooleanFromXML(object, "vertical", true);
    for(auto&& objectChild : object.GetObjects())
    {
      if(objectChild.GetName() == "DockerSplit")
      {
        children.push_back(new DockerSplit(objectChild, docker));
      }
      else if(objectChild.GetName() == "DockerContainer")
      {
        children.push_back(new DockerContainer(objectChild, docker));
      }
      else
      {
        Log::Error("Unknown component in DockerSplit. Component=", objectChild.GetName());
      }
    }
  }

  DockerSplit::~DockerSplit()
  {
    for(auto&& child : children)
      delete child;
  }

  void DockerSplit::Render(GUIRenderer* renderer) const
  {
    for(auto&& child : children)
    {
      child->Render(renderer);
    }
  }

  void DockerSplit::Update(float timeElapsed)
  {
    for(auto&& child : children)
    {
      child->Update(timeElapsed);
    }
  }

  void DockerSplit::OnEvent(Event& event, const Vec2& componentPos)
  {
    for(auto&& child : children)
    {
      child->OnEvent(event, componentPos);
    }
  }

  Component* DockerSplit::GetComponentByNameNoCast(const std::string& name)
  {
    for(auto&& child : children)
    {
      Component* comp = child->GetComponentByNameNoCast(name);
      if(comp)
        return comp;
    }
    return nullptr;
  }

  void DockerSplit::SetGUIScene(GUIScene* scene)
  {
    for(auto&& child : children)
    {
      child->SetGUIScene(scene);
    }
  }

  void DockerSplit::SetPosition(const Vec2& _position)
  {
    position = _position;
    Vec2 offset = position;
    for(auto&& child : children)
    {
      child->SetPosition({floor(offset.x), floor(offset.y)});
      if(vertical)
        offset.x += child->GetSize().x;
      else
        offset.y += child->GetSize().y;
    }
  }

  void DockerSplit::SetSize(const Vec2& _size)
  {
    size = _size;
    Vec2 sumSize{0,0};
    for(auto&& child : children)
    {
      if(vertical)
        sumSize.x += child->GetSize().x;
      else
        sumSize.y += child->GetSize().y;
    }
    for(auto&& child : children)
    {
      if(vertical)
        child->SetSize({floor(_size.x * (child->GetSize().x / sumSize.x)), _size.y});
      else
        child->SetSize({_size.x, floor(_size.y * (child->GetSize().y / sumSize.y))});
    }
  }
}
