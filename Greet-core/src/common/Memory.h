#pragma once

#include <memory>

#define DELETE_COPY_AND_MOVE(CLASS_NAME) \
  CLASS_NAME(const CLASS_NAME&) = delete; \
  CLASS_NAME(CLASS_NAME&&) = delete; \
  CLASS_NAME& operator=(const CLASS_NAME&) = delete; \
  CLASS_NAME& operator=(CLASS_NAME&&) = delete;

#define DELETE_CONSTRUCTOR_DESTRUCTOR(CLASS_NAME) \
  CLASS_NAME() = delete; \
  ~CLASS_NAME() = delete; \

namespace Greet
{
  template <typename T>
  using Ref = std::shared_ptr<T>;

  template <typename T, typename... Args>
  Ref<T> NewRef(const Args&... args)
  {
    return std::make_shared<T>(args...);
  }
}
