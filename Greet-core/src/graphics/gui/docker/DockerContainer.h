#pragma once

#include <graphics/gui/docker/DockerTab.h>
#include <graphics/gui/docker/DockerInterface.h>
#include <graphics/gui/Button.h>
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
      std::vector<DockerTab*> children;
      uint activeTab = 0;

      bool hover = false;
      uint hoverTab = 0;
      Button* button;
      Component* splitIcon;

    public:
      DockerContainer(const XMLObject& object, Docker* docker, DockerSplit* parentSplit);
      DockerContainer(DockerTab* tab, Docker* docker, DockerSplit* parentSplit);
      virtual ~DockerContainer();

      void SelectTab(int index);
      void ClampSelectedTab();
      void HoverTab(int index);
      void UnhoverTab();
      void RemoveTab(int index);

      int GetTabIndex(DockerTab* tab);

      int GetTab(const Vec2& mousePos);
      int GetTabCount();

    public:
      void Render(GUIRenderer* renderer) const override;
      void Update(float timeElapsed) override;
      void OnEvent(Event& event, const Vec2& componentPos) override;

      bool HandleDroppedTab(DockerTab* tab, MouseReleaseEvent& event, const Vec2& componentPos) override;

      Component* GetComponentByNameNoCast(const std::string& name) override;
      Vec2 GetMinSize() const override;

      void SetGUIScene(GUIScene* scene) override;
      void SetPosition(const Vec2& position) override;
      void SetSize(const Vec2& size) override;

    private:
      const Vec2& GetTopSplitPos() const;
      const Vec2& GetBottomSplitPos() const;
      const Vec2& GetLeftSplitPos() const;
      const Vec2& GetRightSplitPos() const;
      const Vec2& GetSplitSize() const;
  };
}
