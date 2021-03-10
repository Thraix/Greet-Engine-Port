#pragma once

#include <scripting/NativeScript.h>
#include <ecs/Entity.h>
#include <event/Event.h>
#include <utils/Resource.h>

namespace Greet
{
  class NativeScriptHandler final : public Resource
  {
    DELETE_COPY_AND_MOVE(NativeScriptHandler);

    private:
      static std::string includePaths;

    private:
      void* libraryHandle = nullptr;
      bool hasCreatedScript = false;
      bool hasBoundEntity = false;
      Entity entity;
      NativeScript* script = nullptr;

    public:
      NativeScriptHandler(const std::string& scriptCpp);
      NativeScriptHandler(NativeScript* script);
      virtual ~NativeScriptHandler();

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

