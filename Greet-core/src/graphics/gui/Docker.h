#pragma once

#include <vector>
#include <graphics/gui/Container.h>
#include <utils/xml/XML.h>
#include <graphics/gui/docker/DockerSplit.h>

namespace Greet {
  class Docker : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(Docker);

    protected:
      DockerSplit* split;

    public:
      Docker(const XMLObject& object, Component* parent);

      void Render(GUIRenderer* renderer) const override;
      void Update(float timeElapsed) override;
      void OnEvent(Event& event, const Vec2& componentPos) override;
      void OnMeasured() override;
      Component* GetComponentByNameNoCast(const std::string& name) override;

      void SetGUIScene(GUIScene* scene) override;
  };
}
