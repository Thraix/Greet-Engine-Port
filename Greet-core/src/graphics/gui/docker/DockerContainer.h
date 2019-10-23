#pragma once

#include <graphics/gui/docker/DockerTab.h>
#include <graphics/gui/docker/DockerInterface.h>
#include <utils/xml/XML.h>

#include <vector>
#include <variant>

namespace Greet {

  class Docker;

  class DockerContainer : public DockerInterface
  {
    protected:
      bool vertical;
      std::vector<DockerTab*> children;
      uint currentTab = 0;

    public:
      DockerContainer(const XMLObject& object, Docker* docker);
      virtual ~DockerContainer();

      int GetTab(const Vec2& mousePos);

    public:
      void Render(GUIRenderer* renderer) const override;
      void Update(float timeElapsed) override;
      void OnEvent(Event& event, const Vec2& componentPos) override;
      void SetGUIScene(GUIScene* scene) override;
      void SetPosition(const Vec2& position) override;
      void SetSize(const Vec2& size) override;
  };
}
