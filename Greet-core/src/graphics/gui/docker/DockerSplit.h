#pragma once

#include <graphics/gui/docker/DockerContainer.h>

#include <vector>
#include <variant>
#include <utils/xml/XML.h>

namespace Greet {

  class Docker;

  class DockerSplit
  {
    protected:
      Docker* docker;

      bool vertical;
      std::vector<std::variant<DockerSplit, DockerContainer>> children;

    public:
      DockerSplit(const XMLObject& object, Docker* docker);
  };
}
