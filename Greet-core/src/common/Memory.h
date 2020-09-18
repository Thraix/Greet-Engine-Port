#pragma once

#include <memory>

namespace Greet
{
  template <typename T>
  using Ref = std::shared_ptr<T>;
}
