#pragma once

#include <scripting/NativeScript.h>
#include <ecs/Entity.h>
#include <event/Event.h>
#include <utils/Resource.h>

namespace Greet
{
  class NativeScriptHandler final : public Resource
  {
    private:
      static std::string includePaths;

    private:
      void* libraryHandle = nullptr;
      bool hasCreatedScript = false;
      bool hasBoundEntity = false;
      Entity entity;
      Ref<NativeScript> script;

    public:
      NativeScriptHandler(const std::string& scriptCpp);
      ~NativeScriptHandler();

      void ReloadResource() override;
      void BindEntity(const Entity& aEntity);
      bool HasBoundEntity() const;

      void OnCreate();
      void OnUpdate(float timeElapsed);
      void OnEvent(Event& event);
      void OnDestroy();

      static void LoadIncludePaths();
  };
}

