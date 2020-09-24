#pragma once

#include <dlfcn.h>

#include <ecs/Entity.h>
#include <event/Event.h>
#include <utils/Resource.h>
#include <utils/FileUtils.h>
#include <utils/StringUtils.h>

#define REGISTER_NATIVE_SCRIPT(SCRIPT_NAME)  extern "C" SCRIPT_NAME* New() { return new SCRIPT_NAME; }

namespace Greet
{
  struct NativeScript
  {
    Entity entity{nullptr};
    NativeScript(){}
    virtual ~NativeScript(){}

    virtual void OnCreate() {}
    virtual void OnUpdate(float timeElapsed) {}
    virtual void OnEvent(Event& event) {}
    virtual void OnDestroy() {}
  };

  class NativeScriptResource final : public Resource
  {
    void* libraryHandle;
    protected:
      bool hasCreatedScript = false;
      bool hasBoundEntity = false;
      Entity entity;
      Ref<NativeScript> script;
    public:
      NativeScriptResource(const std::string& scriptCpp)
        : Resource{scriptCpp}, entity{nullptr}
      {
        Log::Info("Loading NativeScript: ", scriptCpp);
        ReloadResource();
      }

      ~NativeScriptResource()
      {
        dlclose(libraryHandle);
      }

      void ReloadResource() override
      {
        if(libraryHandle)
          dlclose(libraryHandle);

        std::string soFile = FileUtils::ReplaceExtension(filePath, "so");
        FileUtils::TimeModified soMod = FileUtils::GetTimeModified(soFile);
        FileUtils::TimeModified cppMod = FileUtils::GetTimeModified(filePath);
        if(soMod < cppMod)
        {
          Log::Info("Compiling ", filePath);
          // Compile so
          // TODO: This really needs to keep track of code injection vulnerabilities

          // Unsure if this fully covers this vulnerability
          // There might be some ascii way of symbolizing the " character
          std::string cppSecure = filePath;
          std::string soSecure = soFile;
          StringUtils::ReplaceAll(cppSecure, "\"", "\\\"");
          StringUtils::ReplaceAll(soSecure, "\"", "\\\"");
          std::string syscall = "g++ -fPIC -shared -std=c++17 -D_DEBUG -D_GREET_HOTSWAP -g -I ../../Greet-core/src/ -I ../../deps/includes/ \"" + filePath + "\" -o \"" + soFile + "\"";
          system(syscall.c_str());
        }

        // Load shared library
        libraryHandle = dlopen(soFile.c_str(), RTLD_NOW);
        if(!libraryHandle)
        {
          Log::Error("Failed to load script library: ", filePath);
          Log::Error("dlerror: ", dlerror());
          return;
        }

        // Recreate script
        void* newScript  = dlsym(libraryHandle, "New");
        if(!newScript)
        {
          Log::Error("Failed to load New() function: ", filePath);
          Log::Error("dlerror: ", dlerror());
          return;
        }
        typedef NativeScript*(*CreateFunc)();
        script.reset(CreateFunc(newScript)());

        if(script && hasBoundEntity)
          script->entity = this->entity;
        if(hasCreatedScript)
          script->OnCreate();
      }

      void BindEntity(const Entity& aEntity)
      {
        entity = aEntity;
        if(script)
          script->entity = aEntity;
        hasBoundEntity = true;
      }

      bool HasBoundEntity() const { return hasBoundEntity; }

      void OnCreate()
      {
        if(script)
          script->OnCreate();
        hasCreatedScript = true;
      }

      void OnUpdate(float timeElapsed)
      {
        if(script)
          script->OnUpdate(timeElapsed);
      }

      void OnEvent(Event& event)
      {
        if(script)
          script->OnEvent(event);
      }

      void OnDestroy() {if(script) script->OnDestroy(); }
  };
};
