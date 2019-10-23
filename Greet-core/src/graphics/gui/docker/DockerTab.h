#pragma once

#include <graphics/gui/Component.h>
#include <graphics/gui/docker/DockerInterface.h>
#include <utils/xml/XML.h>

#include <vector>
#include <variant>

namespace Greet {

  class Docker;

  class DockerTab : public DockerInterface 
  {
    protected:
      std::string title;
      Component* component;

    public:
      DockerTab(const XMLObject& object, Docker* parent);
      virtual ~DockerTab();

    public:
        void SetPosition(const Vec2& position) override;
        void SetSize(const Vec2& position) override;
        void Render(GUIRenderer* renderer) const override;
  };
}
