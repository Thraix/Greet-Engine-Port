#pragma once

#include <utility>
#include <string>
#include <utils/FileUtils.h>
#include <functional>

namespace Greet
{
  struct HotswapResource
  {
    HotswapResource(const std::string& filePath, std::function<void(const std::string&)> reloadFunction)
      : filePath{filePath}, modDate{FileUtils::GetTimeModified(filePath)},reloadFunction{reloadFunction}
    {}
    std::string filePath;
    TimeModified modDate;
    std::function<void(const std::string&)> reloadFunction;
    void CheckResource()
    {
      TimeModified newDate = FileUtils::GetTimeModified(filePath);
      if(modDate < newDate)
      {
        modDate = newDate;
        reloadFunction(filePath);
      }
    }
  };

  struct HotSwapping
  {
    static std::vector<HotswapResource> resources;

    template <typename T>
    static auto AddHotswapResourceHelper(T* type, const std::string& filePath, int)
      -> decltype(type->ReloadResource(filePath), std::declval<void>())
    {
      resources.push_back({filePath, std::bind(&T::ReloadResource,std::ref(*type),std::placeholders::_1)});
    }

    template <typename T>
    static void AddHotswapResourceHelper(T*, const std::string&, long long)
    {
      static_assert(sizeof(T) != sizeof(T), "The given resource hasn't implemented the function \'void ReloadResource(const std::string&)\'");
    }
    
    template <typename T>
    static void AddHotswapResource(T* t, const std::string& filePath)
    {
      AddHotswapResourceHelper(t,filePath,0);
    }

    template <typename Resource>
    static auto MoveHotswapResourceHelper(Resource* oldRes, Resource* newRes, const std::string& filePath, int)
      -> decltype(oldRes->ReloadResource(filePath), std::declval<void>())
    {
    }

    static void CheckResources()
    {
      for(auto&& resource : resources)
      {
        resource.CheckResource();
      }
    }
  };
}
