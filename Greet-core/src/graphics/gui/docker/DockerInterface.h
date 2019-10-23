#pragma once

#include <math/Vec2.h>
#include <graphics/renderers/GUIRenderer.h>

namespace Greet
{
  class Docker;

  class DockerInterface
  {
    protected:
      Docker* docker;
      Vec2 position;
      Vec2 size;
    public:
      DockerInterface(Docker* docker);
      virtual ~DockerInterface(){}


      const Vec2& GetPosition() const { return position; }
      const Vec2& GetSize() const { return size; }

    public:
      virtual void Render(GUIRenderer* renderer) const = 0;
      virtual void SetPosition(const Vec2& position) = 0;
      virtual void SetSize(const Vec2& position) = 0;
  };
}

