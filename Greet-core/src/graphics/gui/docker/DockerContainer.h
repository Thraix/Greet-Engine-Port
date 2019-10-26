#pragma once

#include <graphics/gui/docker/DockerTab.h>
#include <graphics/gui/docker/DockerInterface.h>
#include <utils/xml/XML.h>

#include <vector>
#include <variant>

namespace Greet {

  class Docker;
  class DockerSplit;

  class DockerContainer : public DockerInterface
  {
    protected:
      bool vertical;
      DockerSplit* parentSplit; 
      std::vector<DockerTab*> children;
      uint currentTab = 0;

      bool hover = false;
      uint hoverTab = 0;

    public:
      DockerContainer(const XMLObject& object, Docker* docker, DockerSplit* parentSplit);
      virtual ~DockerContainer();

      void SelectTab(int index);
      void ClampSelectedTab();
      void HoverTab(int index);
      void UnhoverTab();
      void RemoveTab(int index);

      int GetTabIndex(DockerTab* tab);

      int GetTab(const Vec2& mousePos);

    public:
      void Render(GUIRenderer* renderer) const override;
      void Update(float timeElapsed) override;
      void OnEvent(Event& event, const Vec2& componentPos) override;

       void HandleDroppedTab(DockerTab* tab, MouseReleaseEvent& event, const Vec2& componentPos) override;

      Component* GetComponentByNameNoCast(const std::string& name) override;

      void SetGUIScene(GUIScene* scene) override;
      void SetPosition(const Vec2& position) override;
      void SetSize(const Vec2& size) override;
  };
}
