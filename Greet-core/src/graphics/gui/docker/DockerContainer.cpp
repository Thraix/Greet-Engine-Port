#include "DockerContainer.h"

#include <graphics/fonts/FontManager.h>
#include <graphics/gui/Docker.h>
#include <input/Input.h>

namespace Greet
{
  DockerContainer::DockerContainer(const XMLObject& object, Docker* docker, DockerSplit* parent)
    : DockerInterface{docker, parent}, activeTab{0}
  {
    weight = GUIUtils::GetFloatFromXML(object, "weight", 1.0);
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
    if(children.size() > 0)
      children[0]->ShowTab();
    button = new Button(docker->GetTabButton(), nullptr);
    button->AddStyle("active", Style{"active", docker->GetTabButton()});
    splitIcon = new Component(docker->GetSplitIconStyle(), nullptr);
    splitIcon->Remeasure();
  }

  DockerContainer::DockerContainer(DockerTab* tab, Docker* docker, DockerSplit* parent)
    : DockerInterface{docker, parent}, activeTab{0}
  {
    children.push_back(tab);
    tab->SetContainer(this);
    button = new Button(docker->GetTabButton(), nullptr);
    button->AddStyle("active", Style{"active", docker->GetTabButton()});
    splitIcon = new Component(docker->GetSplitIconStyle(), nullptr);
    splitIcon->Remeasure();
  }

  DockerContainer::~DockerContainer()
  {
    for(auto&& child : children)
      delete child;
    delete button;
    delete splitIcon;
  }

  void DockerContainer::Render(GUIRenderer* renderer) const
  {
    if(activeTab < 0 || activeTab >= children.size())
      return;

    renderer->PushViewport(position, size);
    children[activeTab]->Render(renderer);

    renderer->PushTranslation(docker->GetPosition() + position);
    for(int i = 0;i<children.size();i++)
    {
      if(i == activeTab)
        button->SetCurrentStyle("active");
      else if(hover && i == hoverTab)
        button->SetCurrentStyle("hover");
      else
        button->SetCurrentStyle("normal");
      button->PreRender(renderer, {(float)i * (float)button->GetWidth(), 0.0f});
      button->SetText(children[i]->GetTitle());
      button->Render(renderer);
      button->PostRender(renderer);
    }
    if(docker->IsHoldingTab() && Utils::IsInside(Input::GetMousePosPixel(), position, size))
    {
      splitIcon->PreRender(renderer, GetTopSplitPos());
      splitIcon->Render(renderer);
      splitIcon->PostRender(renderer);
      splitIcon->PreRender(renderer, GetBottomSplitPos());
      splitIcon->Render(renderer);
      splitIcon->PostRender(renderer);
      splitIcon->PreRender(renderer, GetLeftSplitPos());
      splitIcon->Render(renderer);
      splitIcon->PostRender(renderer);
      splitIcon->PreRender(renderer, GetRightSplitPos());
      splitIcon->Render(renderer);
      splitIcon->PostRender(renderer);
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
    if(activeTab < 0 || activeTab >= children.size())
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
    children[activeTab]->OnEvent(event, componentPos + GetTabOffset());
  }

  void DockerContainer::HandleDroppedTab(DockerTab* tab, MouseReleaseEvent& event, const Vec2& componentPos)
  {
    Vec2 mousePos = event.GetPosition() - position;
    DockerSplit* split = static_cast<DockerSplit*>(parent);
    int index = split->GetDockerIndex(this);
    bool splitContainer = false;
    bool vertical = false;
    bool before = false;
    if(Utils::IsInside(mousePos, GetTopSplitPos(), GetSplitSize()))
    {
      AddSplitAbove(tab);
      return;
    }
    else if(Utils::IsInside(mousePos, GetBottomSplitPos(), GetSplitSize()))
    {
      AddSplitBelow(tab);
      return;
    }
    else if(Utils::IsInside(mousePos, GetLeftSplitPos(), GetSplitSize()))
    {
      AddSplitLeft(tab);
      return;
    }
    else if(Utils::IsInside(mousePos, GetRightSplitPos(), GetSplitSize()))
    {
      AddSplitRight(tab);
      return;
    }
    // Only move inside the current container
    if(tab->GetContainer() == this)
    {
      int tabIndex = GetTab(event.GetPosition() - componentPos);
      if(tabIndex < children.size())
      {
        int current = GetTabIndex(tab);
        if(current == tabIndex)
          return;
        if(current > tabIndex)
          std::rotate(children.begin()+tabIndex, children.begin() + current, children.begin() + current+1);
        else
          std::rotate(children.begin()+current, children.begin() + 1, children.begin() + tabIndex+1);
        UnhoverTab();
        SelectTab(tabIndex);
        return;
      }
      return;
    }
    else
    {
      DockerContainer* tabContainer = tab->GetContainer();

      int tabIndex = GetTab(event.GetPosition() - componentPos);
      int tabIndexContainer = tabContainer->GetTabIndex(tab);
      AddTab(tabIndex, tab);

      // Fix this container
      tab->GetContainer()->RemoveTab(tab);
      tab->SetContainer(this);
      SelectTab(tabIndex);
      return;
    }
  }

  void DockerContainer::SelectTab(int i)
  {
    ASSERT(i >= 0 && i < children.size(), "Index out of bound");
    if(i != activeTab)
    {
      if(activeTab >= 0 && activeTab < children.size())
        children[activeTab]->HideTab();
      children[i]->ShowTab();
    }
    activeTab = i;
  }

  void DockerContainer::ClampSelectedTab()
  {
    if(activeTab >= children.size())
    {
      SelectTab(children.size() - 1);
    }
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

  void DockerContainer::AddTab(int index, DockerTab* tab)
  {
    ASSERT(index >= 0 && index <= children.size(), "Index out of bound");
    children.insert(children.begin() + index, tab);
    tab->SetPosition(position + GetTabOffset());
    tab->SetSize(size - GetTabOffset());
  }

  void DockerContainer::RemoveTab(int i)
  {
    ASSERT(i >= 0 && i < children.size(), "Index out of bound");
    children.erase(children.begin() + i);

    DockerSplit* split = static_cast<DockerSplit*>(parent);
    if(children.size() == 0)
      docker->MarkDirty();
    else
      ClampSelectedTab();
  }


  void DockerContainer::RemoveTab(DockerTab* tab)
  {
    RemoveTab(GetTabIndex(tab));
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
      child->SetPosition(_position + GetTabOffset());
    }
  }

  void DockerContainer::SetSize(const Vec2& _size)
  {
    size = _size;
    for(auto&& child : children)
    {
      child->SetSize(_size - GetTabOffset());
    }
    button->Measure();
    button->MeasureFill(size, {1.0f,1.0f});
  }

  DockerTab* DockerContainer::GetTab(const std::string& tabName) const
  {
    for(auto&& child : children)
    {
      if(child->GetTitle() == tabName)
        return child;
    }
    return nullptr;
  }

  const Vec2& DockerContainer::GetTopSplitPos() const
  {
    // TODO: Store all this in a variable
    Vec2 center = size*0.5;
    const Vec2& dockerSize = GetSplitSize();
    float spacing = 10;
    static Vec2 pos;
    pos = center - dockerSize*0.5 - Vec2{0, dockerSize.h + spacing};
    return pos;
  }

  const Vec2& DockerContainer::GetBottomSplitPos() const
  {
    // TODO: Store all this in a variable
    Vec2 center = size*0.5;
    const Vec2& dockerSize = GetSplitSize();
    float spacing = 10;
    static Vec2 pos;
    pos = center - dockerSize*0.5 + Vec2{0, dockerSize.h + spacing};
    return pos;
  }

  const Vec2& DockerContainer::GetLeftSplitPos() const
  {
    // TODO: Store all this in a variable
    Vec2 center = size*0.5;
    const Vec2& dockerSize = GetSplitSize();
    float spacing = 10;
    static Vec2 pos;
    pos = center - dockerSize*0.5 - Vec2{dockerSize.w + spacing, 0};
    return pos;
  }

  const Vec2& DockerContainer::GetRightSplitPos() const
  {
    // TODO: Store all this in a variable
    Vec2 center = size*0.5;
    const Vec2& dockerSize = GetSplitSize();
    float spacing = 10;
    static Vec2 pos;
    pos = center - dockerSize*0.5 + Vec2{dockerSize.w + spacing, 0};
    return pos;
  }

  const Vec2& DockerContainer::GetSplitSize() const
  {
    // TODO: Store all this in a variable
    static Vec2 splitSize = {30,30};
    return splitSize;
  }

  const Vec2& DockerContainer::GetTabOffset() const
  {
    static Vec2 offset = {0, button->GetHeight()};
    return offset;
  }

  void DockerContainer::AddSplitLeft(DockerTab* tab)
  {
    AddSplit(tab, false, true);
  }

  void DockerContainer::AddSplitRight(DockerTab* tab)
  {
    AddSplit(tab, false, false);
  }

  void DockerContainer::AddSplitAbove(DockerTab* tab)
  {
    AddSplit(tab, true, true);
  }

  void DockerContainer::AddSplitBelow(DockerTab* tab)
  {
    AddSplit(tab, true, false);
  }

  void DockerContainer::AddSplit(DockerTab* tab, bool vertical, bool before)
  {
    DockerSplit* split = static_cast<DockerSplit*>(parent);
    int index = split->GetDockerIndex(this);

    if(tab->GetContainer() == this && children.size() == 1)
    {
      return;
    }
    if(tab->GetContainer() != nullptr)
      tab->GetContainer()->RemoveTab(tab);

    DockerContainer* container = new DockerContainer(tab, docker, split);
    DockerSplit* newSplit;
    if(before)
      newSplit = new DockerSplit({container, this}, docker, split, vertical);
    else
      newSplit = new DockerSplit({this, container}, docker, split, vertical);

    container->SetWeight(1);
    SetWeight(1);
    newSplit->SetPosition(position);
    newSplit->SetSize(size);
    newSplit->SetWeight(weight);
    split->AddDocker(index, newSplit);
    split->RemoveDocker(index+1);
  }
}
