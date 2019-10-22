#pragma once

#include <vector>
#include <graphics/gui/Container.h>
#include <utils/xml/XML.h>
#include <graphics/gui/docker/DockerSplit.h>

namespace Greet {
  class Docker : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(Docker);

    protected:
      DockerSplit* split;

    public:
      Docker(const XMLObject& object, Component* parent);

      void Render(GUIRenderer* renderer) const override;
  };
}
