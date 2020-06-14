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
    friend class DockerContainer;
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

      void HandleDroppedTab(DockerTab* tab, MouseReleaseEvent& event, const Vec2& componentPos) override;

      void AddDocker(int index, DockerInterface* docker);
      DockerInterface* RemoveDocker(int index);
      void MergeSimilarSplits();

      int GetDockerIndex(DockerInterface* interface);
      int GetDockerCount() const;
      bool IsVertical() const;

      Component* GetComponentByNameNoCast(const std::string& name) override;
      Vec2 GetMinSize() const override;

      void SetPosition(const Vec2& _size) override;
      void SetSize(const Vec2& _size) override;
      void SetGUIScene(GUIScene* scene) override;

      void UpdateWeights();
      void NormalizeWeights();

      void DebugPrint(int indent) override
      {
        std::stringstream ss;
        for(int i = 0;i<indent;i++)
        {
          ss << "-";
        }
        if(vertical)
          ss << "[vertical]";
        else
          ss << "[horizontal]";
        ss << " w=" << weight;
        Log::Info(ss.str());
        for(auto it = children.begin(); it != children.end();++it)
        {
          (*it)->DebugPrint(indent+1);
        }
      }
  };
}
