#pragma once

#include <vector>
#include <graphics/gui/Component.h>
#include <utils/xml/XML.h>

namespace Greet {
  class Container : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(Container);
    protected:
      std::vector<Component*> m_components;
      bool vertical = true;
      float spacing = 10;

    public:
      Container();
      Container(const std::string& name, Component* parent);
      Container(const XMLObject& object, Component* parent);


      virtual ~Container();

      void Measure(const Vec2& emptyParentSpace, const Vec2& percentageFill) override;
      Vec2 GetMeasureFillSize() override;
      float GetMeasureTotalWeight() override;

      virtual void PostConstruction() override;

      virtual void RenderHandle(GUIRenderer* renderer) const override;
      virtual void UpdateHandle(float timeElapsed) override;

      void AddComponent(Component* component);
      Component* RemoveComponent(uint index);
      Component* RemoveComponent(Component* component);
      Component* GetComponent(uint index);
      size_t GetComponentCount() const;

      bool IsVertical() const;
      Container& SetVertical(bool v);
      Container& SetSpacing(float spacing);

      virtual float GetWrapWidth() const override;
      virtual float GetWrapHeight() const override;

      void OnMousePressEventHandler(MousePressEvent& event, const Vec2& componentPos) override;
      void OnMouseMoveEventHandler(MouseMoveEvent& event, const Vec2& componentPos) override;

      virtual void SetGUIScene(GUIScene* scene) override;

      virtual Component* GetComponentByNameNoCast(const std::string& name) override;

    private:
      void MeasureChildren(const Vec2& weightTotals);
  };
}
