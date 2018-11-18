#pragma once

#include <vector>
#include "Component.h"
#include "GUIMouseListener.h"
#include <utils/xml/XML.h>

namespace Greet {
  class Container : public Component 
  {
    protected:
      std::vector<Component*> m_components;

    public:
      Container();
      Container(const XMLObject& object, Component* parent);
      virtual ~Container();

      void RenderHandle(GUIRenderer* renderer) const override;

      void AddComponent(Component* component);
      Component* RemoveComponent(uint index);
      Component* RemoveComponent(Component* component);
      Component* GetComponent(uint index);

      virtual Component* OnMousePressed(const MousePressedEvent& event, const Vec2& translatedPos) override;
      virtual void OnMouseReleased(const MouseReleasedEvent& event, const Vec2& translatedPos) override { Log::Warning("Called mouse release event on Container");}
      virtual Component* OnMouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos) override;
  };
}
