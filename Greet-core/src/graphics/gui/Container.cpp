#include "Container.h"

#include "GUIUtils.h"
#include <utils/Utils.h>
#include <utils/AABBUtils.h>
#include <graphics/gui/Component.h>

namespace Greet {

  Container::Container()
    : Container(XMLObject(), nullptr)
  {

  }

  Container::Container(const XMLObject& object, Component* parent)
    : Component(object, parent), vertical(true)
  {
    vertical = GUIUtils::GetBooleanFromXML(object,"verticalAlign",true);
    for (uint i = 0;i < object.GetObjectCount();i++)
    {
      AddComponent(GUIUtils::GetComponent(object.GetObject(i), this));
    }
  }

  Container::~Container()
  {
    for(auto it{m_components.begin()}; it != m_components.end();++it)
    {
      delete *it;
    }
  }

  void Container::RenderHandle(GUIRenderer* renderer) const
  {
    renderer->PushMatrix(Mat3::Translate(pos));
    // Render rest of component
    Render(renderer);

    for(auto it{m_components.begin()}; it != m_components.end();++it)
    {
      (*it)->PreRender(renderer, (*it)->GetMargin().LeftTop() + GetTotalPadding());
      (*it)->RenderHandle(renderer);
      (*it)->PostRender(renderer);
    }
    renderer->PopMatrix();
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
    if (component == NULL)
    {
      Log::Warning("Cannot add NULL to component");
      return;
    }
    if(m_components.size() > 0)
    {
      Component* last = *m_components.rbegin();
      if(vertical)
        component->SetPosition(Vec2(0,last->GetPosition().y + last->GetSize().y + last->GetMargin().GetHeight() + 10));
      else
        component->SetPosition(Vec2(last->GetPosition().x + last->GetSize().x + last->GetMargin().GetWidth() + 10,0));
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

  Component* Container::GetComponentByName(const std::string& name)
  {
    Component* comp = Component::GetComponentByName(name);
    if(comp)
      return comp;
    for(auto it{m_components.begin()}; it != m_components.end();++it)
    {
      comp = (*it)->GetComponentByName(name);
      if(comp)
        return comp;
    }
    return nullptr;
  }

  Component* Container::OnMousePressed(const MousePressedEvent& event, const Vec2& translatedPos)
  {
    for(auto it = m_components.rbegin(); it != m_components.rend();++it)
    {
      Component* c{*it};
      if(c->IsMouseInside(translatedPos - GetTotalPadding() - c->GetMargin().LeftTop()))
      {
        Component* focused= c->OnMousePressed(event, translatedPos - GetTotalPadding() - c->GetPosition() - c->GetMargin().LeftTop());
        if(focused)
          return focused;
      }
    }
    return Component::OnMousePressed(event,translatedPos);
  }

  Component* Container::OnMouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos)
  {
    for(auto it = m_components.rbegin(); it != m_components.rend();++it)
    {
      Component* c{*it};
      if(c->IsMouseInside(translatedPos - GetTotalPadding() - c->GetMargin().LeftTop()))
      {
        Component* hovered = c->OnMouseMoved(event, translatedPos - GetTotalPadding() - c->GetPosition() - c->GetMargin().LeftTop());
        if(hovered)
          return hovered;
      }
    }
    return Component::OnMouseMoved(event,translatedPos);
  }

  void Container::Resized()
  {
    for(auto it{m_components.begin()}; it != m_components.end();++it)
    {
      (*it)->ParentResized(size);
    }
  }
}
