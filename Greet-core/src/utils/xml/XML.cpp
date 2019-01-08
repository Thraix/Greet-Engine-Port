#include "XML.h"

#include <fstream>
#include <utils/xml/XMLException.h>

namespace Greet
{
  XMLObject XML::FromString(const std::string& string)
  {
    return XMLObject(string);
  }

  XMLObject XML::FromFile(const std::string& fileName)
  {
    std::ifstream file(fileName, std::ios::binary | std::ios::ate);
    if(!file)
      throw XMLException("Could not read file \""+fileName+"\"");
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::string buffer;
    buffer.reserve(size);
    while (!file.eof())
    {
      buffer += file.get();
    }
    return FromString(buffer);
  }
}
