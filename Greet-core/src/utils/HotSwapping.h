#pragma once

#include <utility>
#include <string>
#include <utils/FileUtils.h>
#include <functional>
#include <map>

namespace Greet
{
  struct HotswapResource
  {
    private:
      static uint ids;
    public:
      uint id;
      std::string filePath;
      TimeModified modDate;
      std::function<void(const std::string&)> reloadFunction;

      HotswapResource(const std::string& filePath, std::function<void(const std::string&)> reloadFunction)
        : filePath{filePath}, modDate{FileUtils::GetTimeModified(filePath)},reloadFunction{reloadFunction},id{ids++}
      {}

      template <typename T>
      auto MoveResourceHelper(T* type, int)
        -> decltype(type->ReloadResource(filePath), std::declval<void>())
      {
        reloadFunction = std::bind(&T::ReloadResource,std::ref(*type),std::placeholders::_1);
      }

      template <typename T>
      void MoveResourceHelper(T*, long long)
      {
        static_assert(sizeof(T) != sizeof(T), "The given resource hasn't implemented the function \'void ReloadResource(const std::string&)\'");
      }

      template <typename T>
      void MoveResource(T* t)
      {
        MoveResourceHelper(t, 0);
      }

      void CheckResource()
      {
        TimeModified newDate = FileUtils::GetTimeModified(filePath);
        if(modDate < newDate)
        {
          modDate = newDate;
          reloadFunction(filePath);
        }
      }

      bool operator<(const HotswapResource& resource)
      {
        return id < resource.id;
      }
  };

  struct HotSwapping
  {
    static std::map<uint, HotswapResource> resources;

    template <typename T>
    static auto AddHotswapResourceHelper(T* type, const std::string& filePath, int)
      -> decltype(type->ReloadResource(filePath), resources.emplace(0, std::declval<HotswapResource>()).first)
    {
      HotswapResource resource = {filePath, std::bind(&T::ReloadResource,std::ref(*type),std::placeholders::_1)};
      return resources.emplace(resource.id, resource).first;
    }

    template <typename T>
    static std::map<uint, HotswapResource>::iterator AddHotswapResourceHelper(T*, const std::string&, long long)
    {
      static_assert(sizeof(T) != sizeof(T), "The given resource hasn't implemented the function \'void ReloadResource(const std::string&)\'");
      return resources.end();
    }

    template <typename T>
    static std::map<uint, HotswapResource>::iterator AddHotswapResource(T* t, const std::string& filePath)
    {
      return AddHotswapResourceHelper(t,filePath,0);
    }

    static void CheckResources()
    {
      for(auto&& resource : resources)
      {
        resource.second.CheckResource();
      }
    }
  };
}
