#pragma once

#include <vector>
#include <graphics/gui/Component.h>
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

      virtual void RenderHandle(GUIRenderer* renderer) const override;
      virtual void UpdateHandle(float timeElapsed) override;

      void AddComponent(Component* component);
      Component* RemoveComponent(uint index);
      Component* RemoveComponent(Component* component);
      Component* GetComponent(uint index);

      Component* GetComponentByName(const std::string& name) override;


      virtual Component* OnMousePressed(const MousePressedEvent& event, const Vec2& translatedPos) override;
      virtual Component* OnMouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos) override;

      virtual void Resized() override;
  };
}
