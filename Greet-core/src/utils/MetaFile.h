#pragma once

#include <map>
#include <string>
#include <fstream>
#include <utils/StringUtils.h>
#include <logging/Log.h>

namespace Greet
{
  class MetaFile
  {
    private:
      std::string filepath;
      std::map<std::string, std::string> values; // map<variable, value>

    public:
      MetaFile(const std::string& filepath)
        : filepath{filepath}
      {
        if(StringUtils::EndsWith(filepath, ".meta"))
        {
          std::ifstream stream(filepath);

          if(!stream.is_open())
          {
            Log::Error("Could not find meta file: ", filepath);
            return;
          }

          std::string line;
          while(std::getline(stream, line))
          {
            if(line.empty())
              continue;

            size_t pos = line.find("=");
            if(pos == std::string::npos)
            {
              Log::Warning("Meta file line does not contain \'=\'");
              continue;
            }

            std::string key = line.substr(0,pos);
            std::string value = line.substr(pos+1);
            if(key.length() == 0)
            {
              Log::Warning("Meta file key is empty");
              continue;
            }
            if(!values.emplace(key, value).second)
            {
              Log::Warning("Meta file key is defined twice: ", key);
            }
          }
        }
      }

      std::string GetValue(const std::string& key, const std::string& val) const
      {
        auto it = values.find(key);
        if(it != values.end())
          return it->second;
        return val;
      }
  };
}
