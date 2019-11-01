#include "DockerSplit.h"

#include <graphics/gui/Docker.h>

namespace Greet
{
  DockerSplit::DockerSplit(const XMLObject& object, Docker* docker, DockerSplit* parent)
    : DockerInterface{docker, parent}
  {
    vertical = GUIUtils::GetBooleanFromXML(object, "vertical", false);
    for(auto&& objectChild : object.GetObjects())
    {
      if(objectChild.GetName() == "DockerSplit")
      {
        children.push_back(new DockerSplit(objectChild, docker, this));
      }
      else if(objectChild.GetName() == "DockerContainer")
      {
        children.push_back(new DockerContainer(objectChild, docker, this));
      }
      else
      {
        Log::Error("Unknown component in DockerSplit. Component=", objectChild.GetName());
      }
    }
  }

  DockerSplit::DockerSplit(DockerSplit* split, Docker* docker, DockerSplit* parent, bool vertical)
    : DockerInterface{docker, parent}, children{split}, vertical{vertical}
  {
    split->SetParent(this);
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
    int vecIndex = vertical ? 1 : 0;
    Vec2 offset{position};

    Vec2 p{0,0};
    Vec2 s{(float)docker->edgeWidth, (float)docker->edgeWidth};

    // Code for rendering edges, needed for later
#if 0
    p[vecIndex] = -docker->edgeWidth;
    s[1-vecIndex] = size[1-vecIndex];
    for(int i = 0;i<children.size()-1;i++)
    {
      offset[vecIndex] += children[i]->GetSize()[vecIndex] + docker->edgeWidth;
      renderer->PushTranslation(offset);
      renderer->SubmitRect(p, s, {1,1,1,1}, false);
      renderer->PopTranslation();
    }
#endif
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
          mousePos[vecIndex] -= children[i]->GetSize()[vecIndex] + docker->edgeWidth;
          if(mousePos[vecIndex] >= -docker->edgeWidth && mousePos[vecIndex] < 0)
          {
            grabbingEdge = true;
            grabbedEdgeIndex = i;
            grabPos = e.GetPosition()[vecIndex] - componentPos[vecIndex];
            grabSize = children[i]->GetSize()[vecIndex];
            event.AddFlag(EVENT_FOCUSED | EVENT_HANDLED);
            return;
          }
        }
      }
      Vec2 pos = componentPos;
      int vecIndex = vertical ? 1 : 0;
      for(auto&& child : children)
      {
        if(child->IsMouseInside(e.GetPosition() - pos))
          child->OnEvent(event, pos);
        pos[vecIndex] += child->GetSize()[vecIndex] + docker->edgeWidth;
      }
      return;
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
      if(grabbingEdge)
      {
        Vec2 mousePos = e.GetPosition() - componentPos;

        DockerInterface* dockerThis = children[grabbedEdgeIndex];
        DockerInterface* dockerNext = children[grabbedEdgeIndex+1];
        float oldWeightThis = children[grabbedEdgeIndex]->GetWeight();
        float oldWeightNext = children[grabbedEdgeIndex+1]->GetWeight();

        Vec2 sizeThis = dockerThis->GetSize();
        Vec2 sizeNext = dockerNext->GetSize();

        int vecIndex = vertical ? 1 : 0;

        float offset = mousePos[vecIndex] - grabPos;

        float totalWeight = (oldWeightThis + oldWeightNext);
        float totalSize = sizeThis[vecIndex] + sizeNext[vecIndex];

        sizeThis[vecIndex] = grabSize + offset;
        float minSizeThis = dockerThis->GetMinSize()[vecIndex];
        float minSizeNext = dockerNext->GetMinSize()[vecIndex];
        Math::Clamp(&sizeThis[vecIndex], minSizeThis, totalSize - minSizeNext);


        float weightThis = totalWeight * sizeThis[vecIndex] / totalSize;

        dockerThis->SetWeight(weightThis);
        dockerNext->SetWeight(totalWeight - weightThis);

        SetSize(size);
        return;
      }
      Vec2 pos = componentPos;
      int vecIndex = vertical ? 1 : 0;
      for(auto&& child : children)
      {
        child->OnEvent(event, pos);
        pos[vecIndex] += child->GetSize()[vecIndex] + docker->edgeWidth;
      }
      return;
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
    {
      if(grabbingEdge)
      {
        grabbingEdge = false;
        event.AddFlag(EVENT_UNFOCUSED);
        return;
      }
      Vec2 pos = componentPos;
      int vecIndex = vertical ? 1 : 0;
      MouseReleaseEvent& e = static_cast<MouseReleaseEvent&>(event);
      for(auto&& child : children)
      {
        child->OnEvent(event, pos);
        pos[vecIndex] += child->GetSize()[vecIndex];
      }
      return;
    }

    Vec2 pos = componentPos;
    int vecIndex = vertical ? 1 : 0;
    for(auto&& child : children)
    {
      child->OnEvent(event, pos);
      pos[vecIndex] += child->GetSize()[vecIndex];
    }
  }

  bool DockerSplit::HandleDroppedTab(DockerTab* tab, MouseReleaseEvent& event, const Vec2& componentPos)
  {
    Vec2 mousePos = event.GetPosition() - componentPos;
    Vec2 position = {0,0};
    for(auto&& child : children)
    {
      if(mousePos.x >= position.x && mousePos.y >= position.y && mousePos.x < position.x + child->GetSize().x && mousePos.y < position.y + child->GetSize().y)
      {
        return child->HandleDroppedTab(tab, event, componentPos + position);
      }
      int vecIndex = vertical ? 1 : 0;
      position[vecIndex] += child->GetSize()[vecIndex];
    }
    return false;
  }

  void DockerSplit::AddContainer(int index, DockerContainer* container)
  {
    children.insert(children.begin() + index, container);
    container->SetWeight(1.0f);

    int vecIndex = vertical ? 1 : 0;

    Vec2 containerSize = size;
    container->SetParent(this);
    container->SetSize(containerSize);
    SetSize(size);
    SetPosition(position);
  }

  void DockerSplit::RemoveDocker(int index)
  {
    ASSERT(index >= 0 && index < children.size(), "Index out of bound");
    DockerInterface* interface = *(children.begin() + index);
    children.erase(children.begin() + index);

    float weight = interface->GetWeight();
    delete interface;

    if(children.size() == 0)
    {
      if(parent != nullptr)
      {
        DockerSplit* split = static_cast<DockerSplit*>(parent);
        static_cast<DockerSplit*>(split)->RemoveDocker(split->GetDockerIndex(this));
      }
    }
    else
    {

      float totalWeight = children.size() + 1 - weight;
      if(totalWeight != 0)
      {
        float upscale = children.size() / totalWeight;
        for(auto&& child : children)
        {
          child->SetWeight(child->GetWeight() * upscale);
        }
      }
      else
      {
        for(auto&& child : children)
        {
          child->SetWeight(1.0f);
        }
      }
      // Resize other components
      SetSize(size);
      SetPosition(position);
    }
  }

  int DockerSplit::GetDockerIndex(DockerInterface* interface)
  {
    for(int i = 0;i<children.size(); ++i)
    {
      if(children[i] == interface)
        return i;
    }
    ASSERT(false, "Docker is not inside DockerSplit");
    return children.size();
  }

  int DockerSplit::GetDockerCount() const
  {
    return children.size();
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

  Vec2 DockerSplit::GetMinSize() const
  {
    Vec2 minSize = {0, 0};
    int vecIndex = vertical ? 1 : 0;
    for(auto&& child : children)
    {
      Vec2 minSizeChild = child->GetMinSize();
      minSize[vecIndex] += minSizeChild[vecIndex];
      minSize[1-vecIndex] = fmax(minSize[1-vecIndex], minSizeChild[1-vecIndex]);
    }
    minSize[vecIndex] += (children.size() - 1) * docker->edgeWidth;
    return minSize;
  }

  bool DockerSplit::IsVertical() const
  {
    return vertical;
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
      offset[vecIndex] += child->GetSize()[vecIndex] + docker->edgeWidth;
    }
  }

  void DockerSplit::SetSize(const Vec2& _size)
  {
    size = _size;
    int vecIndex = vertical ? 1 : 0;
    float totalWeight = children.size();
    int edgeWidths = docker->edgeWidth * (children.size() - 1);
    Vec2 pos = position;

    for(auto&& child : children)
    {
      Vec2 childSize = _size;
      childSize[vecIndex] = floor((_size[vecIndex] - edgeWidths) * child->GetWeight() / totalWeight);
      child->SetSize(childSize);
      child->SetPosition(pos);
      pos[vecIndex] += childSize[vecIndex] + docker->edgeWidth;
    }
  }
}
