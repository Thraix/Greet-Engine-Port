#include "DockerInterface.h"

namespace Greet
{
  DockerInterface::DockerInterface(Docker* docker, DockerInterface* parent)
    : docker{docker}, parent{parent}, position{0,0}, size{1,1}
  {

  }
}
