#include "DockerContainer.h"

#include <graphics/gui/Docker.h>

namespace Greet
{
  DockerContainer::DockerContainer(const XMLObject& object, Docker* docker)
    : docker{docker}
  {}
}
