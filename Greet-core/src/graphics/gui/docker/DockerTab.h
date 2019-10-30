#pragma once

#include <graphics/gui/Component.h>
#include <graphics/gui/docker/DockerInterface.h>
#include <utils/xml/XML.h>

#include <vector>
#include <variant>

namespace Greet {

  class Docker;
  class DockerContainer;

  class DockerTab
  {
    protected:
      Docker* docker;
      DockerContainer* parentContainer;
      Vec2 position;
      Vec2 size;

      std::string title;
      Component* component;

    public:
      DockerTab(const XMLObject& object, Docker* docker, DockerContainer* parent);
      virtual ~DockerTab();

      const std::string& GetTitle() const;
    public:
      void Render(GUIRenderer* renderer) const;
      void Update(float timeElapsed);
      void OnEvent(Event& event, const Vec2& componentPos);

      Component* GetComponentByNameNoCast(const std::string& name);
      DockerContainer* GetContainer();

      void SetContainer(DockerContainer* parentContainer);
      void SetGUIScene(GUIScene* scene);
      void SetPosition(const Vec2& position);
      void SetSize(const Vec2& position);
  };
}
