#include "HotSwapping.h"

namespace Greet
{
    std::map<uint, HotswapResource> HotSwapping::resources;
    uint HotswapResource::ids = 0;
} 

