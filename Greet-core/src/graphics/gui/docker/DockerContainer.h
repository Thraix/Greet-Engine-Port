#pragma once

#include <graphics/gui/docker/DockerTab.h>
#include <utils/xml/XML.h>

#include <vector>
#include <variant>

namespace Greet {

  class Docker;

  class DockerContainer
  {
    protected:
      Docker* docker;

      bool vertical;
      std::vector<DockerTab> children;

    public:
      DockerContainer(const XMLObject& object, Docker* docker);
  };
}
