#pragma once

namespace Greet
{
  class Scene
  {
    public:
      virtual void PreRender() const {}
      virtual void Render() const = 0;
      virtual void PostRender() const {}
      virtual void PreUpdate(float timeElapsed) {}
      virtual void Update(float timeElapsed) = 0;
      virtual void PostUpdate(float timeElapsed) {}


      // TODO: Add mouse,key listeners
  };
}
