#include "Container.h"

#include "GUIUtils.h"
#include <utils/Utils.h>
#include <utils/AABBUtils.h>

namespace Greet {

  Container::Container()
    : Container(XMLObject(), nullptr)
  {

  }

  // These are usually the top element so no parent is needed
  Container::Container(const XMLObject& object, Component* parent)
    : Component(object, parent)
  {
    for (uint i = 0;i < object.GetObjectCount();i++)
    {
      AddComponent(GUIUtils::GetComponent(object.GetObject(i), this));
    }
  }

  Container::~Container()
  {

  }

  void Container::PreRender(GUIRenderer* renderer) const
  {
    renderer->PushViewport(pos, size, false);
  }

  void Container::PostRender(GUIRenderer* renderer) const
  {
    renderer->PopViewport();
  }

  void Container::AddComponent(Component* component)
  {
    if (component == NULL)
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

  Content* Container::OnMousePressed(const MousePressedEvent& event, const Vec2& translatedPos)
  {
    for(auto it = m_components.rbegin(); it != m_components.rend();++it)
    {
      Component* c{*it};
      if(c->IsMouseInside(translatedPos))
      {
        Content* focused = c->OnMousePressed(event, translatedPos - c->GetPosition());
        if(focused)
          return focused;
      }
    }
    return nullptr;
  }

  Content* Container::OnMouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos)
  {
    for(auto it = m_components.rbegin(); it != m_components.rend();++it)
    {
      Component* c{*it};
      if(c->IsMouseInside(translatedPos))
      {
        Content* hovered = c->OnMouseMoved(event, translatedPos - c->GetPosition());
        if(hovered)
          return hovered;
      }
    }
    return nullptr;
  }
}
