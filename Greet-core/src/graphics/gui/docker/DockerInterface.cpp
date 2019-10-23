#include "DockerInterface.h"

namespace Greet
{
  DockerInterface::DockerInterface(Docker* docker)
    : docker{docker}, position{0,0}, size{1,1}
  {

  }
}
