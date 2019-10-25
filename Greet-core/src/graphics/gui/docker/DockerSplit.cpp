#include "DockerSplit.h"

#include <graphics/gui/Docker.h>

namespace Greet
{
  DockerSplit::DockerSplit(const XMLObject& object, Docker* docker)
    : DockerInterface{docker}
  {
    vertical = GUIUtils::GetBooleanFromXML(object, "vertical", false);
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
      if((vertical && mousePos.x >= 0 && mousePos.x < size.w) || (!vertical && mousePos.y >= 0 && mousePos.y < size.h))
      {
        int vecIndex = vertical ? 1 : 0;
        for(int i = 0;i<children.size()-1;i++)
        {
          mousePos -= children[i]->GetSize();
          if(mousePos[vecIndex] >= -5 && mousePos[vecIndex] < 5)
          {
            grabbingEdge = true;
            grabbedEdgeIndex = i;
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

        DockerInterface* dockerThis = children[grabbedEdgeIndex];
        DockerInterface* dockerNext = children[grabbedEdgeIndex+1];

        Vec2 sizeThis = dockerThis->GetSize();
        Vec2 posNext = dockerNext->GetPosition();
        Vec2 sizeNext = dockerNext->GetSize();

        int vecIndex = vertical ? 1 : 0;

        float offset = e.GetDeltaPosition()[vecIndex];

        sizeThis[vecIndex] += offset;
        posNext[vecIndex] += offset;
        sizeNext[vecIndex] -= offset;

        dockerThis->SetSize(sizeThis);
        dockerNext->SetPosition(posNext);
        dockerNext->SetSize(sizeNext);
        return;
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
    {
      if(grabbingEdge)
      {
        grabbingEdge = false;
        return;
      }
    }

    Vec2 pos = componentPos;
    int vecIndex = vertical ? 1 : 0;
    for(auto&& child : children)
    {
      child->OnEvent(event, pos);
      pos[vecIndex] += child->GetSize()[vecIndex];
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
    int vecIndex = vertical ? 1 : 0;
    for(auto&& child : children)
    {
      child->SetPosition({floor(offset.x), floor(offset.y)});
      offset[vecIndex] += child->GetSize()[vecIndex];
    }
  }

  void DockerSplit::SetSize(const Vec2& _size)
  {
    size = _size;
    Vec2 sumSize{0,0};
    int vecIndex = vertical ? 1 : 0;

    for(auto&& child : children)
    {
      sumSize[vecIndex] += child->GetSize()[vecIndex];
    }
    for(auto&& child : children)
    {
      Vec2 childSize = _size;
      childSize[vecIndex] = floor(_size[vecIndex] * (child->GetSize()[vecIndex] / sumSize[vecIndex]));
      child->SetSize(childSize);
    }
  }
}
