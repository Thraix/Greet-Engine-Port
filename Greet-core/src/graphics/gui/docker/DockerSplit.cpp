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
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      Vec2 mousePos = e.GetPosition() - componentPos;
      if((vertical && mousePos.y >= 0 && mousePos.y < size.h) || (!vertical && mousePos.x >= 0 && mousePos.x < size.w))
      {
        for(int i = 0;i<children.size()-1;i++)
        {
          mousePos -= children[i]->GetSize();
          if(vertical && mousePos.x >= -5 && mousePos.x < 5)
          {
            grabbingEdge = true;
            grabbedEdgeIndex = i;
            grabbedPos = mousePos.x;
            grabbedSize = children[i]->GetSize().x;
            return;
          }
          else if(!vertical && mousePos.y >= -5 && mousePos.y < 5)
          {
            grabbingEdge = true;
            grabbedEdgeIndex = i;
            grabbedPos = mousePos.y;
            grabbedSize = children[i]->GetSize().y;
            return;
          }
        }
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      if(grabbingEdge)
      {
        MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
        Vec2 mousePos = e.GetPosition() - componentPos;
        const Vec2& sizeThis = children[grabbedEdgeIndex]->GetSize();
        const Vec2& posNext = children[grabbedEdgeIndex+1]->GetPosition();
        const Vec2& sizeNext = children[grabbedEdgeIndex+1]->GetSize();
        if(vertical)
        {
          float offset = e.GetDX();
          children[grabbedEdgeIndex]->SetSize({sizeThis.x + offset, sizeThis.y});
          children[grabbedEdgeIndex+1]->SetPosition({posNext.x + offset, posNext.y});
          children[grabbedEdgeIndex+1]->SetSize({sizeNext.x - offset, sizeNext.y});
        }
        else
        {
          float offset = e.GetDY();
          children[grabbedEdgeIndex]->SetSize({sizeThis.x, sizeThis.y + offset});
          children[grabbedEdgeIndex+1]->SetPosition({posNext.x, posNext.y + offset});
          children[grabbedEdgeIndex+1]->SetSize({sizeNext.x, sizeNext.y - offset});
        }
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
    {
      grabbingEdge = false;
    }

    Vec2 pos = componentPos;
    for(auto&& child : children)
    {
      child->OnEvent(event, pos);
      if(vertical)
        pos.x += child->GetSize().x;
      else
        pos.y += child->GetSize().y;
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
