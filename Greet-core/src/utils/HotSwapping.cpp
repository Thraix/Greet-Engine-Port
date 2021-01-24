#include "HotSwapping.h"

namespace Greet
{
  std::map<uint, Resource*> HotSwapping::resources;

  void HotSwapping::AddHotswapResource(Resource* res)
  {
    Log::Info("Adding hotswapped resource: count=", resources.size() + 1, ", file=", res->filePath);
    resources.emplace(res->id, res);
  }

  void HotSwapping::RemoveHotswapResource(Resource* res)
  {
    Log::Info("Removing hotswapped resource: count=", resources.size() - 1, ", file=", res->filePath);
    ASSERT(resources.erase(res->id) == 1, "Invalid remove size on Hotswapped resource");
  }

  void HotSwapping::CheckResources()
  {
    for(auto&& resource : resources)
    {
      resource.second->CheckResource();
    }
  }
}
