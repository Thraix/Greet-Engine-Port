#pragma once

#include <ecs/Config.h>
#include <ecs/ECSManager.h>
#include <common/Memory.h>

namespace Greet
{
  class Entity
  {
    friend class ECSManager;

    private:
      Ref<ECSManager> manager;
      EntityID id;

    public:
      Entity(const Ref<ECSManager>& manager)
        : manager{manager}, id{0}
      {}

      Entity(const Ref<ECSManager>& manager, EntityID id)
        : manager{manager}, id{id}
      {}

      operator EntityID() const { return id; }
      operator bool() const { return manager->ValidEntity(id); }

      void Destroy()
      {
        manager->DestroyEntity(id);
      }

      template <typename Component, typename... Args>
      inline Component& AddComponent(Args... args)
      {
        return manager->AddComponent<Component>(id, args...);
      }

      template <typename... Components>
      std::tuple<Components&...> AddComponents(Components&&... components)
      {
        return manager->AddComponents(id, components...);
      }

      template <typename Component>
      inline void RemoveComponent()
      {
        return manager->RemoveComponent<Component>(id);
      }

      template <typename... Components>
      inline void RemoveComponents()
      {
        return manager->RemoveComponents<Components...>(id);
      }

      template <typename Component>
      inline Component& GetComponent() const
      {
        return manager->GetComponent<Component>(id);
      }

      template <typename Component>
      inline bool HasComponent() const
      {
        return manager->HasComponent<Component>(id);
      }

      void SetID(EntityID aId)
      {
        id = aId;
      }

      EntityID GetID() const
      {
        return id;
      }

      static Entity Create(const Ref<ECSManager>& manager)
      {
        return {manager, manager->CreateEntity()};
      }
  };
}
