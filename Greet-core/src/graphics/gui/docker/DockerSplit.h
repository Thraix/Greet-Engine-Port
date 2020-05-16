#pragma once

#include <graphics/gui/docker/DockerContainer.h>
#include <graphics/gui/docker/DockerInterface.h>
#include <utils/xml/XML.h>

#include <vector>
#include <variant>

namespace Greet {

  class Docker;

  class DockerSplit : public DockerInterface
  {
    protected:
      bool vertical;
      std::vector<DockerInterface*> children;

      float grabPos;
      float grabSize;
      bool grabbingEdge = false;
      int grabbedEdgeIndex;

    public:
      DockerSplit(const XMLObject& object, Docker* docker, DockerSplit* parent);
      DockerSplit(DockerSplit* split, Docker* docker, DockerSplit* parent, bool vertical);
      DockerSplit(const std::vector<DockerInterface*>& children, Docker* docker, DockerSplit* parent, bool vertical);
      virtual ~DockerSplit();

      void Render(GUIRenderer* renderer) const override;
      void Update(float timeElapsed) override;
      void OnEvent(Event& event, const Vec2& componentPos) override;

      bool HandleDroppedTab(DockerTab* tab, MouseReleaseEvent& event, const Vec2& componentPos) override;

      void AddDocker(int index, DockerInterface* docker, bool fixSize = true);
      void RemoveDocker(int index, bool shouldDelete = true, bool fixSize = true);
      void MergeSimilarSplits();

      int GetDockerIndex(DockerInterface* interface);
      int GetDockerCount() const;
      bool IsVertical() const;

      Component* GetComponentByNameNoCast(const std::string& name) override;
      Vec2 GetMinSize() const override;

      void SetPosition(const Vec2& _size) override;
      void SetSize(const Vec2& _size) override;
      void SetGUIScene(GUIScene* scene) override;
  };
}
