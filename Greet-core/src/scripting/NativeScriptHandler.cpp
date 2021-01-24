#include "NativeScriptHandler.h"

#include <dlfcn.h>

#include <utils/FileUtils.h>
#include <utils/StringUtils.h>
#include <utils/xml/XML.h>

namespace Greet
{
  std::string NativeScriptHandler::includePaths;

  NativeScriptHandler::NativeScriptHandler(const std::string& scriptCpp)
    : Resource{scriptCpp}, entity{nullptr}
  {
    Log::Info("Loading NativeScript: ", scriptCpp);
    ReloadResource();
  }

  NativeScriptHandler::~NativeScriptHandler()
  {
    if(script)
      delete script;
    if(libraryHandle)
      dlclose(libraryHandle);
  }

  void NativeScriptHandler::ReloadResource()
  {
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
      std::string syscall = "g++ -fPIC -shared -std=c++17 -D_DEBUG -D_GREET_HOTSWAP -g" + includePaths + " \"" + filePath + "\" -o \"" + soFile + "\"";
      system(syscall.c_str());
    }

    // Free current script
    script = {nullptr};
    if(libraryHandle)
      dlclose(libraryHandle);

    // Load shared library
    libraryHandle = dlopen(soFile.c_str(), RTLD_NOW);

    if(!libraryHandle)
    {
      Log::Error("Failed to load script: ", filePath);
      Log::Error("dlerror: ", dlerror());
      return;
    }

    // Recreate script
    void* newScript  = dlsym(libraryHandle, "New");
    if(!newScript)
    {
      Log::Error("Failed to load New() function (Forgot REGISTER_NATIVE_SCRIPT define?): ", filePath);
      Log::Error("dlerror: ", dlerror());
      dlclose(libraryHandle);
      return;
    }
    if(script)
      delete script;
    typedef NativeScript*(*NewScript)();
    script = NewScript(newScript)();

    if(script && hasBoundEntity)
      script->entity = this->entity;
    if(hasCreatedScript)
      script->OnCreate();
  }

  void NativeScriptHandler::BindEntity(const Entity& aEntity)
  {
    entity = aEntity;
    if(script)
      script->entity = aEntity;
    hasBoundEntity = true;
  }

  bool NativeScriptHandler::HasBoundEntity() const
  {
    return hasBoundEntity;
  }

  void NativeScriptHandler::OnCreate()
  {
    if(script)
      script->OnCreate();
    hasCreatedScript = true;
  }

  void NativeScriptHandler::OnUpdate(float timeElapsed)
  {
    if(script)
      script->OnUpdate(timeElapsed);
  }

  void NativeScriptHandler::OnEvent(Event& event)
  {
    if(script)
      script->OnEvent(event);
  }

  void NativeScriptHandler::OnDestroy()
  {
    if(script)
      script->OnDestroy();
  }

  void NativeScriptHandler::LoadIncludePaths()
  {
    try
    {
      XMLObject makegen = XML::FromFile("makegen.xml");
      std::string target;
      for(auto&& o : makegen.GetObjects())
      {
        if(o.GetName() == "target")
        {
          target = o.GetText();
          break;
        }
      }
      XMLObject configuration;
      for(auto&& o : makegen.GetObjects())
      {
        if(o.GetName() == "configuration" && o.GetAttribute("name", "") == target)
        {
          configuration = o;
        }
      }
      for(auto&& o : configuration.GetObjects())
      {
        if(o.GetName() == "includedir")
        {
          std::string path = o.GetText();
          StringUtils::ReplaceAll(path, "\"", "\\\"");
          includePaths += " -I \"" + path + "\"";
        }
      }
    }
    catch(XMLException& e)
    {
      Log::Error("Could not load makegen build file");
    }
  }
}
