#include "Loaders.h"

#include <graphics/textures/CubeMap.h>
#include <graphics/textures/Texture2D.h>
#include <graphics/textures/TextureManager.h>
#include <utils/json/JSONLoader.h>

namespace Greet
{
  void Loaders::LoadTextures(const std::string& loader)
  {
    JSONObject object = JSONLoader::LoadJSON(loader);
    if(!object.HasArrayKey("textures"))
    {
      Log::Error("Texture loader doesn't contain any textures");
      return;
    }

    std::string basePath = object.GetValue("basePath", "res/textures/");
    const JSONArray& array = object.GetArray("textures");
    for(int i = 0;i<array.Size();i++)
    {
      const JSONObject& object = array.GetObject(i);
      if(!object.HasKey("name"))
      {
        Log::Error("Texture in TextureLoader didn't contain a name\n", object);
        continue;
      }
      std::string path = basePath;
      TextureParams params;

      // If no path is specified use the name and add png after it
      if(!object.HasKey("path"))
        path += object.GetValue("name")+".png";
      else
        path += object.GetValue("path");

      std::string type = object.GetValue("type", "Texture2D");

      if(object.HasKey("filter"))
      {
        if(type == "CubeMap")
          Log::Warning("CubeMap doesn't support changing filter mode");
        std::string filter = object.GetValue("filter");
        if(filter == "none")
          params.filter = TextureFilter::NONE;
        else if(filter == "linear")
          params.filter = TextureFilter::LINEAR;
        else if(filter == "nearest")
          params.filter = TextureFilter::NEAREST;
      }

      if(object.HasKey("wrap"))
      {
        if(type == "CubeMap")
          Log::Warning("CubeMap doesn't support changing wrap mode");
        std::string wrap = object.GetValue("wrap");
        if(wrap == "none")
          params.wrap = TextureWrap::NONE;
        else if(wrap == "clampEdge")
          params.wrap = TextureWrap::CLAMP_TO_EDGE;
        else if(wrap == "clampBorder")
          params.wrap = TextureWrap::CLAMP_TO_BORDER;
        else if(wrap == "mirrorRepeat")
          params.wrap = TextureWrap::MIRRORED_REPEAT;
        else if(wrap == "repeat")
          params.wrap = TextureWrap::REPEAT;
        else if(wrap == "mirrorClampEdge")
          params.wrap = TextureWrap::MIRROR_CLAMP_TO_EDGE;
      }

      if(object.HasKey("format"))
      {
        if(type == "CubeMap")
          Log::Warning("CubeMap doesn't support changing internal format");
        std::string wrap = object.GetValue("format");
        if(wrap == "depth")
          params.internalFormat = TextureInternalFormat::DEPTH_COMPONENT;
        else if(wrap == "stencil")
          params.internalFormat = TextureInternalFormat::DEPTH_STENCIL;
        else if(wrap == "red")
          params.internalFormat = TextureInternalFormat::RED;
        else if(wrap == "rgb")
          params.internalFormat = TextureInternalFormat::RGB;
        else if(wrap == "rgba")
          params.internalFormat = TextureInternalFormat::RGBA;
      }

      if(type == "Texture2D")
        TextureManager::Add(object.GetValue("name"), Texture2D(path, params));
      else if(type == "CubeMap")
        TextureManager::Add(object.GetValue("name"), CubeMap(path));
      else
        Log::Error("Unsupported texture type \"", type, "\"");
    }
  }
}
