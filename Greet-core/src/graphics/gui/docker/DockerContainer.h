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
      std::vector<DockerTab*> children;
      uint activeTab = 0;

      bool hover = false;
      uint hoverTab = 0;

    public:
      DockerContainer(const XMLObject& object, Docker* docker, DockerSplit* parentSplit);
      DockerContainer(DockerTab* tab, Docker* docker, DockerSplit* parentSplit);
      virtual ~DockerContainer();

      void SelectTab(int index);
      void ClampSelectedTab();
      void HoverTab(int index);
      void UnhoverTab();
      void AddTab(int index, DockerTab* tab);
      void RemoveTab(int index);
      void RemoveTab(DockerTab* tab);

      int GetTabIndex(DockerTab* tab);

      int GetTab(const Vec2& mousePos);
      int GetTabCount();

    public:
      void Render(GUIRenderer* renderer) const override;
      void Update(float timeElapsed) override;
      void OnEvent(Event& event, const Vec2& componentPos) override;

      void HandleDroppedTab(DockerTab* tab, MouseReleaseEvent& event, const Vec2& componentPos) override;

      Component* GetComponentByNameNoCast(const std::string& name) override;
      Vec2 GetMinSize() const override;

      void SetGUIScene(GUIScene* scene) override;
      void SetPosition(const Vec2& position) override;
      void SetSize(const Vec2& avSize, bool abRemeasure) override;

      DockerTab* GetTab(const std::string& tabName) const override;

      void LoadFrameStyle(const MetaFile& metaFile) override;

      inline void AddSplitLeft(DockerTab* tab);
      inline void AddSplitRight(DockerTab* tab);
      inline void AddSplitAbove(DockerTab* tab);
      inline void AddSplitBelow(DockerTab* tab);

    private:
      const Vec2& GetTopSplitPos() const;
      const Vec2& GetBottomSplitPos() const;
      const Vec2& GetLeftSplitPos() const;
      const Vec2& GetRightSplitPos() const;
      const Vec2& GetSplitSize() const;

      void AddSplit(DockerTab* tab, bool vertical, bool before);

      const Vec2& GetTabOffset() const;

      void DebugPrint(int indent) override
      {
        std::stringstream ss;
        for(int i = 0;i<indent;i++)
        {
          ss << "-";
        }
        if(children.size() == 0)
          ss << "No children";
        else
          ss << children[0]->GetTitle();
        ss << " w=" << weight;
        Log::Info(ss.str());
      }
  };
}
