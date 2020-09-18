#pragma once

#include <common/Memory.h>
#include <ecs/ECSManager.h>
#include <ecs/Entity.h>
#include <event/Event.h>
#include <event/ViewportEvent.h>

namespace Greet
{
  class Scene
  {
    private:
      Ref<ECSManager> manager;
    public:
      Scene();

      Entity AddEntity();
      void RemoveEntity(const Entity& entity);
      void RemoveEntity(EntityID entity);

      virtual void PreRender();
      virtual void Render() const;
      virtual void PostRender();
      virtual void PreUpdate(float timeElapsed);
      virtual void Update(float timeElapsed);
      virtual void PostUpdate(float timeElapsed);
      virtual void OnEvent(Event& event)
      {
        if(EVENT_IS_TYPE(event, EventType::VIEWPORT_RESIZE))
        {
          ViewportResize((ViewportResizeEvent&) event);
        }
      }
      virtual void ViewportResize(ViewportResizeEvent& event);
  };
}
