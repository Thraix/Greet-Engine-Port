#pragma once

#include <graphics/gui/Component.h>
#include <graphics/gui/docker/DockerInterface.h>
#include <utils/xml/XML.h>

#include <vector>
#include <variant>

namespace Greet {

  class Docker;

  class DockerTab : public DockerInterface 
  {
    protected:
      std::string title;
      Component* component;

    public:
      DockerTab(const XMLObject& object, Docker* parent);
      virtual ~DockerTab();

      const std::string& GetTitle() const;
    public:
      void Render(GUIRenderer* renderer) const override;
      void Update(float timeElapsed) override;
      void OnEvent(Event& event, const Vec2& componentPos) override;
      void SetGUIScene(GUIScene* scene) override;
      void SetPosition(const Vec2& position) override;
      void SetSize(const Vec2& position) override;
  };
}
