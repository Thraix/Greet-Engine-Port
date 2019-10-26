#pragma once

#include <graphics/gui/Component.h>
#include <graphics/gui/docker/DockerInterface.h>
#include <utils/xml/XML.h>

#include <vector>
#include <variant>

namespace Greet {

  class Docker;
  class DockerContainer;

  class DockerTab : public DockerInterface 
  {
    protected:
      std::string title;
      Component* component;
      DockerContainer* parentContainer;

    public:
      DockerTab(const XMLObject& object, Docker* parent, DockerContainer* parentContainer);
      virtual ~DockerTab();

      const std::string& GetTitle() const;
    public:
      void Render(GUIRenderer* renderer) const override;
      void Update(float timeElapsed) override;
      void OnEvent(Event& event, const Vec2& componentPos) override;

      void HandleDroppedTab(DockerTab* tab, MouseReleaseEvent& event, const Vec2& componentPos) override;

      Component* GetComponentByNameNoCast(const std::string& name) override;
      DockerContainer* GetContainer();
      void SetContainer(DockerContainer* container);

      void SetGUIScene(GUIScene* scene) override;
      void SetPosition(const Vec2& position) override;
      void SetSize(const Vec2& position) override;
  };
}
