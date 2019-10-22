#pragma once

#include <graphics/gui/Component.h>
#include <utils/xml/XML.h>

#include <vector>
#include <variant>

namespace Greet {

  class Docker;

  class DockerTab
  {
    protected:
      Docker* docker;

      std::string title;
      Component* component;

    public:
      DockerTab(const XMLObject& object, Docker* parent);
      virtual ~DockerTab();
  };
}
