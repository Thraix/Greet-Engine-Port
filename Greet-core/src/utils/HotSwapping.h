#pragma once

#include <utility>
#include <string>
#include <utils/Utils.h>
#include <utils/FileUtils.h>
#include <functional>
#include <map>
#include <memory>
#include <utils/Resource.h>

namespace Greet
{
  struct HotSwapping
  {
    DELETE_CONSTRUCTOR_DESTRUCTOR(HotSwapping);

    private:
      static std::map<uint, Resource*> resources;

    public:
      static void AddHotswapResource(Resource* res);
      static void RemoveHotswapResource(Resource* res);
      static void CheckResources();
  };
}
