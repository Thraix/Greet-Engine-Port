#pragma once

#include <graphics/gui/docker/DockerContainer.h>
#include <graphics/gui/docker/DockerInterface.h>

#include <vector>
#include <variant>
#include <utils/xml/XML.h>

namespace Greet {

  class Docker;

  class DockerSplit : public DockerInterface
  {
    protected:
      bool vertical;
      std::vector<DockerInterface*> children;

      bool grabbingEdge = false;
      int grabbedEdgeIndex;

    public:
      DockerSplit(const XMLObject& object, Docker* docker);
      virtual ~DockerSplit();

      void Render(GUIRenderer* renderer) const override;
      void Update(float timeElapsed) override;
      void OnEvent(Event& event, const Vec2& componentPos) override;

      void HandleDroppedTab(DockerTab* tab, MouseReleaseEvent& event, const Vec2& componentPos) override;

      Component* GetComponentByNameNoCast(const std::string& name) override;

      void SetPosition(const Vec2& _size) override;
      void SetSize(const Vec2& _size) override;
      void SetGUIScene(GUIScene* scene) override;
  };
}