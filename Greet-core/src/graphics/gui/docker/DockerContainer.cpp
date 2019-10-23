#include "DockerContainer.h"

#include <graphics/gui/Docker.h>

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
