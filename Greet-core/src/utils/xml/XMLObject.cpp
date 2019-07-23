#include "XMLObject.h"

#include <cstring>
#include <utils/xml/XMLException.h>
#include <utils/StringUtils.h>
#include <logging/Log.h>

namespace Greet
{
  XMLObject::XMLObject(const std::string& string)
  {
    int pos = 0;
    int line = 1;
    if(!ReadHead(string, &pos, &line))
      ReadBodyTail(string, &pos, &line);
  }

  XMLObject::XMLObject(const std::string& string, int* pos, int* line)
  {
    if (!ReadHead(string, pos, line))
      ReadBodyTail(string, pos, line);
  }

  XMLObject::XMLObject(const std::string& name, const std::map<std::string, std::string>& attributes, const std::string& text)
    :name(name), attributes(attributes), text(text)
  {

  }

  XMLObject::XMLObject(const std::string& name, const std::map<std::string, std::string>& attributes, const std::vector<XMLObject>& objects)
    : name(name), attributes(attributes), objects(objects)
  {

  }

  bool XMLObject::HasAttribute(const std::string& property) const
  {
    return attributes.find(property) != attributes.end();
  }

  const std::string& XMLObject::GetAttribute(const std::string& property) const
  {
    auto it = attributes.find(property);
    if (it == attributes.end())
      throw XMLException((std::string("Attribute could not be found \"") + property + "\".").c_str());
    return it->second;
  }

  const std::string& XMLObject::GetAttribute(const std::string& property, const std::string& defaultValue) const
  {
    auto it = attributes.find(property);
    if (it == attributes.end())
      return defaultValue;
    return it->second;
  }

  unsigned int XMLObject::GetObjectCount() const
  {
    return objects.size();
  }

  const XMLObject& XMLObject::GetObject(unsigned int i) const
  {
    if (i >= objects.size())
      throw XMLException((std::string("XML index out of bounds \"") + std::to_string(i) + "\".").c_str());

    return objects[i];
  }

  const std::vector<XMLObject>& XMLObject::GetObjects() const 
  {
    return objects;
  }

  const std::string& XMLObject::GetName() const
  {
    return name;
  }

  const std::string& XMLObject::GetText() const
  {
    return text;
  }

  void XMLObject::SetName(const std::string& name)
  {
    if(StringUtils::IsWord(name))
      this->name = name;
    else
      Log::Error("XML Head can only be made up of letters");
  }

  void XMLObject::SetText(const std::string& text)
  {
    this->text = text;
  }

  void XMLObject::AddAttribute(const std::string& property, const std::string& value)
  {
    if(StringUtils::IsWord(property))
      attributes.emplace(property, value);
    else
      Log::Error("XML property name can only be made up of letters");

  }

  XMLObject XMLObject::GetStrippedXMLObject() const
  {
    if(text == "")
      return XMLObject(name, attributes, objects);
    else
      return XMLObject(name, attributes, text);
  }

  ////////////////////////////////////////////////////////////
  //                                                        //
  // Everything below here handles the reading of xml files //
  //                                                        //
  ////////////////////////////////////////////////////////////


  bool XMLObject::ReadHead(const std::string& string, int* posPointer, int* linePointer)
  {
    // Check if the first character is the start of and xml tag.
    ReadWhiteSpace(string, posPointer, linePointer);
    if (string[*posPointer] != '<')
      throw XMLException("Not an XML Object.", *linePointer);

    // Check if there is a closing tag
    size_t closing = string.find('>');
    if (closing == std::string::npos)
      throw XMLException("No enclosing > for opening tag.", *linePointer);

    // Read the name of the tag
    ReadName(string, posPointer, linePointer);

    //  Read all attributes of the xml tag
    ReadAttributes(string, posPointer, linePointer);

    // Read opening tag
    if (string[*posPointer] == '/')
    {
      (*posPointer)++;
      ReadWhiteSpace(string, posPointer, linePointer);
      if (string[*posPointer] != '>')
        throw XMLException((std::string("Invalid character proceeding / in opening XML Tag \"") + string[*posPointer] + "\".").c_str(), *linePointer);
      (*posPointer)++;
      // nothing more to read.
      return true;
    }

    ReadWhiteSpace(string, posPointer, linePointer);
    if (string[*posPointer] != '>')
      throw XMLException((std::string("Invalid character proceeding attributes in opening XML Tag \"") + string[*posPointer] + "\".").c_str(), *linePointer);
    (*posPointer)++;
    return false;
  }

  void XMLObject::ReadName(const std::string& string, int* posPointer, int* linePointer)
  {
    (*posPointer)++;
    ReadWhiteSpace(string, posPointer, linePointer);
    if (!StringUtils::IsLetter(string[*posPointer]))
      throw XMLException("Invalid XML name. Can only contain letters.", *linePointer);
    name = StringUtils::GetWord(string, *posPointer);
    *posPointer += name.length();
    ReadWhiteSpace(string, posPointer, linePointer);
    if (string[*posPointer] != '/' && string[*posPointer] != '>' && StringUtils::IsWhiteSpace(string[*posPointer]))
    {
      throw XMLException((std::string("Invalid character proceeding name in XML Tag \"") + string[*posPointer] + "\".").c_str(), *linePointer);
    }
  }

  void XMLObject::ReadAttributes(const std::string& string, int* posPointer, int* linePointer)
  {
    ReadWhiteSpace(string, posPointer, linePointer);

    while (string[*posPointer] != '>' && string[*posPointer] != '/')
    {
      ReadAttribute(string, posPointer, linePointer);
    }
  }

  void XMLObject::ReadAttribute(const std::string& string, int* posPointer, int* linePointer)
  {
    // Read property name
    std::string property = ReadXMLName(string, posPointer, linePointer);
    if (property.length() == 0)
      throw XMLException((std::string("Invalid character proceeding name \"") + string[*posPointer] + "\".").c_str(), *linePointer);
    if (attributes.count(property) > 0)
      throw XMLException((std::string("Duplicate property in XML tag \"") + property + "\".").c_str(), *linePointer);
    *posPointer += property.length();
    ReadWhiteSpace(string, posPointer, linePointer);

    // Read =
    if (string[*posPointer] != '=')
      throw XMLException((std::string("Invalid character proceeding property name in XML Tag \"") + string[*posPointer] + "\".").c_str(), *linePointer);
    (*posPointer)++;
    ReadWhiteSpace(string, posPointer, linePointer);

    // Read value
    if (string[*posPointer] != '\"')
      throw XMLException("XML property value is not inside enclosing quotes.", *linePointer);
    (*posPointer)++;
    int valueStart = *posPointer;
    while (string[*posPointer] != '\"') (*posPointer)++;
    std::string value = string.substr(valueStart, (*posPointer) - valueStart);
    ReplacePredefinedEntities(value);
    (*posPointer)++;
    attributes.emplace(property, value);
    ReadWhiteSpace(string, posPointer, linePointer);
  }

  void XMLObject::ReadBodyTail(const std::string& string, int* posPointer, int* linePointer)
  {
    ReadWhiteSpace(string, posPointer, linePointer);
    if (string[*posPointer] != '<')
    {
      ReadText(string, posPointer, linePointer);
      ReadWhiteSpace(string, posPointer, linePointer);
      std::string closeTag = GetClosingTag(string, posPointer, linePointer);
      if (closeTag.length() == 0)
        throw XMLException("Tag after XML Test was not a closing tag. XMLObject doesn't support text and other XMLObjects at the same time.", *linePointer);
      return;
    }
    // Check if we can read the closing tag.
    std::string closeTag = GetClosingTag(string, posPointer, linePointer);
    while (closeTag.length() == 0)
    {
      XMLObject object = XMLObject(string, posPointer, linePointer);
      objects.push_back(object);
      ReadWhiteSpace(string, posPointer, linePointer);
      closeTag = GetClosingTag(string, posPointer, linePointer);
    }
  }

  void XMLObject::ReadText(const std::string& string, int* posPointer, int* linePointer)
  {
    int startPos = *posPointer;
    while (string[*posPointer] != '<') (*posPointer)++;
    text = string.substr(startPos, (*posPointer) - startPos);
    ReplacePredefinedEntities(text);
  }

  void XMLObject::ReadWhiteSpace(const std::string& string, int* posPointer, int* linePointer)
  {
    while (StringUtils::IsWhiteSpace(string[*posPointer])) {
      if (string[*posPointer] == '\n')
        (*linePointer)++;
      (*posPointer)++;
    }
  }


  std::string XMLObject::GetClosingTag(const std::string& string, int* posPointer, int* linePointer)
  {
    int startPos = *posPointer;
    int startLine = *linePointer;
    if (string[(*posPointer)++] != '<')
    {
      *posPointer = startPos;
      *linePointer = startLine;
      return "";
    }
    ReadWhiteSpace(string, posPointer, linePointer);
    if (string[(*posPointer)++] != '/')
    {
      *posPointer = startPos;
      *linePointer = startLine;
      return "";
    }
    ReadWhiteSpace(string, posPointer, linePointer);
    std::string tag = StringUtils::GetWord(string, *posPointer);
    if (tag != name)
      throw XMLException((std::string("Invalid closing tag \"") + tag + "\".").c_str(), 0);
    *posPointer += tag.length();
    ReadWhiteSpace(string, posPointer, linePointer);
    if (string[*posPointer] != '>')
      throw XMLException((std::string("Invalid character in closing tag \"") + string[*posPointer] + "\".").c_str(), 0);
    (*posPointer)++;
    return string.substr(startPos, (*posPointer) - startPos);
  }

  void XMLObject::ReplacePredefinedEntities(std::string& string)
  {
    std::vector<std::pair<std::string, std::string>> entities
    {
      {"&quot;","\""},
      {"&apos;", "\'"},
      {"&lt;", "<"},
      {"&gt;",">"},
      {"&amp;", "&"}
    };
    size_t pos = string.find('&');
    while(pos != std::string::npos)
    {
      for(auto entity : entities)
      {
        if(strncmp(&string[pos], entity.first.c_str(), entity.first.length()) == 0)
        {
          string.replace(pos, entity.first.length(), entity.second);
        }
      }
      pos = string.find('&', pos+1);
    }
  }

  std::string XMLObject::ReadXMLName(const std::string& string, int* posPointer, int* linePointer)
  {
    if(!(StringUtils::IsLetter(string[*posPointer]) || 
          string[*posPointer] == '_' || 
          string[*posPointer] == ':'))
      throw XMLException(std::string("Name doesn't start with a letter."), *linePointer);

    int endPos = *posPointer + 1;
    while (endPos < string.length() && (
          StringUtils::IsLetter(string[endPos]) || 
          string[endPos] == '_' || 
          string[endPos] == '-' || 
          string[endPos] == ':' || 
          string[endPos] == '.')) 
      endPos++;
    return string.substr(*posPointer, endPos - *posPointer);
  }
}
