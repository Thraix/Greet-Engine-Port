#pragma once

#include <memory>

namespace Greet
{
  template <class T>
  using Ref = std::shared_ptr<T>;
  template <class T>
  using Ptr = std::unique_ptr<T>;
}
