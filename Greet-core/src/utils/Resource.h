#pragma once

#include <string>
#include <common/Memory.h>
#include <utils/FileUtils.h>
#include <utils/UUID.h>

namespace Greet
{
  class Resource
  {
    DELETE_COPY_AND_MOVE(Resource);

    friend class HotSwapping;

    private:
      static uint s_Id;
      uint id;
      FileUtils::TimeModified modDate;

    protected:
      std::string filePath;

    public:
      Resource()
        : id{0}
      {}

      Resource(const std::string& filePath);

      virtual ~Resource();

      void CheckResource();
      virtual void ReloadResource() {}

      friend bool operator<(const Resource& r1, const Resource& r2)
      {
        return r1.id < r2.id;
      }
  };
}

