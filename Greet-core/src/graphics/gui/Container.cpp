#include "Container.h"

#include <graphics/gui/GUIUtils.h>
#include <utils/Utils.h>
#include <utils/AABBUtils.h>
#include <graphics/gui/Component.h>
#include <cstdlib>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(Container);

  Container::Container()
    : Container(XMLObject(), nullptr)
  {

  }

  Container::Container(const std::string& name, Component* parent)
    : Component{name, parent}, vertical{true}, spacing{10}
  {
    AddStyleVariables(
      StylingVariables
      {
        .floats = {{"spacing", &spacing}},
        .bools = {{"vertical", &vertical}}
      }
    );
  }

  Container::Container(const XMLObject& object, Component* parent)
    : Component(object, parent), vertical(true)
  {
    AddStyleVariables(
      StylingVariables
      {
        .floats = {{"spacing", &spacing}},
        .bools = {{"vertical", &vertical}}
      }
    );
    LoadStyles(object);
    for (uint i = 0;i < object.GetObjectCount();i++)
    {
      Component* component = ComponentFactory::GetComponent(object.GetObject(i), this);
      if(StringUtils::StartsWith(component->GetName(),"Failed#"))
        Log::Error("Could not load Component, could not understand type. (type=",object.GetObject(i).GetName(),")");
      else
        AddComponent(component);
    }
  }

  Container::~Container()
  {
    for(auto it{m_components.begin()}; it != m_components.end();++it)
    {
      delete *it;
    }
  }

  void Container::PostConstruction()
  {
    for(auto&& comp : m_components)
    {
      comp->PostConstruction();
    }
  }

  void Container::MeasureChildren(const Vec2& weightTotals)
  {
    Vec2 pos{0, 0};
    int spacings = std::max((int)m_components.size() - 1, 0);

    Vec2 totalSize = GetPadding().GetSize() + GetBorder().GetSize();
    if(vertical)
      totalSize.h += spacing * spacings;
    else
      totalSize.w += spacing * spacings;
    for(auto&& child : m_components)
    {
      child->Measure({0, 0}, {0, 0});
      child->SetPosition(pos);
      if(vertical)
        pos.y += child->GetSize().h + spacing;
      else
        pos.x += child->GetSize().w + spacing;
      if(child->GetWidthType() != GUISize::Type::Weight && !vertical)
        totalSize.w += child->GetSize().w;
      if(child->GetHeightType() != GUISize::Type::Weight && vertical)
        totalSize.h += child->GetSize().h;
    }

    totalSize.w = totalSize.w > width.size ? width.size : totalSize.w;
    totalSize.h = totalSize.h > height.size ? height.size : totalSize.h;

    pos = {0, 0};
    for(auto&& child : m_components)
    {
      if(vertical)
      {
        child->Measure({(width.size - totalSize.w), (height.size - totalSize.h) / weightTotals.h}, {1, child->GetSizeValue().h});
      }
      else
      {
        child->Measure({(width.size - totalSize.w) / weightTotals.w, height.size - totalSize.h}, {1, child->GetSizeValue().h});
      }
      child->SetPosition(pos);
      if(vertical)
        pos.y += child->GetHeight() + spacing;
      else
        pos.x += child->GetWidth() + spacing;
    }
  }

  void Container::Measure(const Vec2& emptyParentSpace, const Vec2& percentageFill)
  {
    bool childHasWidthWeight = false;
    bool childHasHeightWeight = false;
    Vec2 totalWeights{0, 0};

    for(auto&& child : m_components)
    {
      if(child->GetWidthType() == GUISize::Type::Weight)
      {
        totalWeights.w += child->GetSizeValue().w;
        childHasWidthWeight = true;
      }
      if(child->GetHeightType() == GUISize::Type::Weight)
      {
        totalWeights.h += child->GetSizeValue().h;
        childHasWidthWeight = true;
      }
    }
    Component::Measure(emptyParentSpace, percentageFill);

    MeasureChildren(totalWeights);

    bool remeasureChildren = true;
    while(remeasureChildren)
    {
      remeasureChildren = false;
      if(width.type == GUISize::Type::Wrap)
      {
        int wrapSize = GetWrapWidth();
        if(wrapSize != width.size)
        {
          width.size = wrapSize;
          if(childHasWidthWeight)
            remeasureChildren = true;
        }
      }
      if(height.type == GUISize::Type::Wrap)
      {
        int wrapSize = GetWrapHeight();
        if(wrapSize != height.size)
        {
          height.size = wrapSize;
          if(childHasHeightWeight)
            remeasureChildren = true;
        }
      }
      if(remeasureChildren)
      {
        MeasureChildren(totalWeights);
      }
    }
  }

  Vec2 Container::GetMeasureFillSize()
  {
    float usedSpace = 0;

    for(auto&& comp : m_components)
    {
      if(this->vertical)
        usedSpace += comp->GetMargin().GetHeight() + spacing;
      else
        usedSpace += comp->GetMargin().GetWidth() + spacing;

      if(this->vertical && comp->GetHeightType() != GUISize::Type::Weight)
        usedSpace += comp->GetSize().h;
      else if(!this->vertical && comp->GetWidthType() != GUISize::Type::Weight)
        usedSpace += comp->GetSize().w;
    }
    if(usedSpace > 0)
      usedSpace -= spacing; // Remove the spacing after the last one
    if(vertical)
    {
      float height = GetContentSize().h - usedSpace;
      if(height < 1)
        height = 1;
      return Vec2(GetContentSize().w, height);
    }
    else
    {
      float width = GetContentSize().w - usedSpace;
      if(width < 1)
        width = 1;
      return Vec2(width, GetContentSize().h);
    }
  }

  float Container::GetMeasureTotalWeight()
  {
    float totalWeight = 0;
    for(auto&& comp : m_components)
    {
      if(vertical && comp->GetHeightType() == GUISize::Type::Weight)
        totalWeight += comp->GetSizeValue().h;
      else if(!vertical && comp->GetWidthType() == GUISize::Type::Weight)
        totalWeight += comp->GetSizeValue().w;
    }
    return totalWeight;
  }

  void Container::RenderHandle(GUIRenderer* renderer) const
  {
    renderer->PushTranslation(pos);
    // Render rest of component
    Render(renderer);

    for(auto it{m_components.begin()}; it != m_components.end();++it)
    {
      (*it)->PreRender(renderer, (*it)->GetMargin().LeftTop() + GetTotalPadding());
      (*it)->RenderHandle(renderer);
      (*it)->PostRender(renderer);
    }
    renderer->PopTranslation();
  }

  void Container::UpdateHandle(float timeElapsed)
  {
    Update(timeElapsed);

    for(auto it{m_components.begin()}; it != m_components.end();++it)
    {
      (*it)->UpdateHandle(timeElapsed);
    }
  }

  void Container::AddComponent(Component* component)
  {
    if (component == nullptr)
    {
      Log::Warning("Cannot add NULL to component");
      return;
    }
    m_components.push_back(component);
  }

  Component* Container::RemoveComponent(uint index)
  {
    if (index >= m_components.size())
    {
      Log::Warning("Index out of bound.");
      return NULL;
    }
    auto it = m_components.begin() + index;
    m_components.erase(it);
    return *it;
  }

  Component* Container::RemoveComponent(Component* component)
  {
    for (auto it = m_components.begin(); it != m_components.end();++it)
    {
      if (*it == component)
      {
        m_components.erase(it);
        return component;
      }
    }
    Log::Warning("Couldn't find component");
    return NULL;
  }

  Component* Container::GetComponent(uint index)
  {
    if (index >= m_components.size())
    {
      Log::Warning("Index out of bound.");
      return NULL;
    }
    return *(m_components.begin() + index);
  }

  Component* Container::GetComponentByNameNoCast(const std::string& name)
  {
    Component* comp = Component::GetComponentByNameNoCast(name);
    if(comp)
      return comp;
    for(auto it{m_components.begin()}; it != m_components.end();++it)
    {
      comp = (*it)->GetComponentByNameNoCast(name);
      if(comp)
        return comp;
    }
    return nullptr;
  }

  size_t Container::GetComponentCount() const
  {
    return m_components.size();
  }

  float Container::GetWrapWidth() const
  {
    float width = 0;
    if(vertical)
    {
      for(auto&& child : m_components)
      {
        if(child->GetWidthType() != GUISize::Type::Weight)
          width = std::max(width, child->GetSize().w);
      }
    }
    else
    {
      width = spacing * std::max(((int)m_components.size() - 1), 0);
      for(auto&& child : m_components)
      {
        if(child->GetWidthType() != GUISize::Type::Weight)
          width += child->GetSize().w;
      }
    }
    return width + GetPadding().GetWidth() + GetBorder().GetWidth();
  }

  float Container::GetWrapHeight() const
  {
    float height = 0;
    if(vertical)
    {
      height = spacing * std::max(((int)m_components.size() - 1), 0);
      for(auto&& child : m_components)
      {
        if(child->GetHeightType() != GUISize::Type::Weight)
          height += child->GetSize().h;
      }
    }
    else
    {
      for(auto&& child : m_components)
      {
        if(child->GetHeightType() != GUISize::Type::Weight)
          height = std::max(height, child->GetSize().h);
      }
    }
    return height + GetPadding().GetHeight() + GetBorder().GetHeight();
  }

  bool Container::IsVertical() const
  {
    return vertical;
  }

  Container& Container::SetVertical(bool v)
  {
    vertical = v;
    Remeasure();
    return *this;
  }

  Container& Container::SetSpacing(float spacing)
  {
    this->spacing = spacing;
    Remeasure();
    return *this;
  }

  void Container::SetGUIScene(GUIScene* scene)
  {
    Component::SetGUIScene(scene);
    for(auto&& component : m_components)
      component->SetGUIScene(scene);
    guiScene = scene;
  }

  void Container::OnMousePressEventHandler(MousePressEvent& event, const Vec2& componentPos)
  {
    for(auto it = m_components.rbegin(); it != m_components.rend();++it)
    {
      Component* c{*it};
      if(c->IsMouseInside(event.GetPosition() - (componentPos + GetTotalPadding() + c->GetMargin().LeftTop() + c->GetPosition())))
      {
        c->OnEventHandler(event, componentPos + GetTotalPadding() + c->GetMargin().LeftTop() + c->GetPosition());
        return;
      }
    }
    Component::OnMousePressEventHandler(event, componentPos);
  }

  void Container::OnMouseMoveEventHandler(MouseMoveEvent& event, const Vec2& componentPos)
  {
    Component::OnMouseMoveEventHandler(event, componentPos);
    if(!UsingMouse())
    {
      for(auto it = m_components.rbegin(); it != m_components.rend();++it)
      {
        (*it)->OnEventHandler(event, componentPos + GetTotalPadding() + (*it)->GetMargin().LeftTop() + (*it)->GetPosition());
      }
    }
  }
}
