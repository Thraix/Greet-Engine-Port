#include "Loaders.h"

#include <graphics/textures/CubeMap.h>
#include <graphics/textures/Texture2D.h>
#include <graphics/textures/TextureManager.h>
#include <utils/json/JSONLoader.h>

using namespace Greet;

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

    // If no path is specified use the name and add png after it
    if(!object.HasKey("path"))
      path += object.GetValue("name")+".png";
    else
      path += object.GetValue("path");

    if(object.GetValue("type", "Texture2D") == "Texture2D")
      TextureManager::Add(object.GetValue("name"), Texture2D(path));
    else
      TextureManager::Add(object.GetValue("name"), CubeMap(path));
  }
}
