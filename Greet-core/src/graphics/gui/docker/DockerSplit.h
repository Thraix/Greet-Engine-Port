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

    public:
      DockerSplit(const XMLObject& object, Docker* docker);
      virtual ~DockerSplit();

      void SetPosition(const Vec2& _size) override;
      void SetSize(const Vec2& _size) override;
      void Render(GUIRenderer* renderer) const override;
  };
}
