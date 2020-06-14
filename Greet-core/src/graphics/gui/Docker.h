#pragma once

#include <vector>
#include <graphics/gui/Container.h>
#include <utils/xml/XML.h>
#include <graphics/gui/docker/DockerSplit.h>
#include <graphics/fonts/FontManager.h>

namespace Greet {

  class Docker : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(Docker);

    protected:
      DockerSplit* split;
      DockerTab* grabbedTab;
      float grabbedDistance = 0;
      XMLObject tabButton;
      XMLObject splitIconStyle;
      bool childrenChanged;
    public:
      Vec4 edgeColor;
      int edgeWidth;
      int edgeBorderSize;
      Vec4 edgeBorderColor;

    public:
      Docker(const XMLObject& object, Component* parent);

      void GrabDockerTab(DockerTab* tab);
      void HandleDroppedTab(MouseReleaseEvent& event, const Vec2& componentPos);

      const XMLObject& GetTabButton() const;
      const XMLObject& GetSplitIconStyle() const;

      // Overrided functions
      void Render(GUIRenderer* renderer) const override;
      void Update(float timeElapsed) override;
      void OnEvent(Event& event, const Vec2& componentPos) override;
      void OnMeasured() override;
      Component* GetComponentByNameNoCast(const std::string& name) override;
      void SetGUIScene(GUIScene* scene) override;

      bool IsHoldingTab() const;

      void MarkDirty();

      inline void AddSplitLeft(DockerTab* tab);
      inline void AddSplitRight(DockerTab* tab);
      inline void AddSplitAbove(DockerTab* tab);
      inline void AddSplitBelow(DockerTab* tab);

      DockerTab* GetTab(const std::string& tabName) const;

    private:
      void Split(DockerTab* tab, bool vertical, bool before);
  };
}
