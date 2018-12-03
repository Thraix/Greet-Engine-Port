#include "XMLObject.h"

#include "XMLException.h"
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

  XMLObject::XMLObject(const std::string& name, const std::map<std::string, std::string>& properties, const std::string& text)
    :name(name), properties(properties), text(text)
  {

  }

  XMLObject::XMLObject(const std::string& name, const std::map<std::string, std::string>& properties, const std::vector<XMLObject>& objects)
    : name(name), properties(properties), objects(objects)
  {

  }

  bool XMLObject::HasProperty(const std::string& property) const
  {
    return properties.find(property) != properties.end();
  }

  const std::string& XMLObject::GetProperty(const std::string& property) const
  {
    auto it = properties.find(property);
    if (it == properties.end())
      throw XMLException((std::string("Property could not be found \"") + property + "\".").c_str());
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
    this->text= text;
  }

  void XMLObject::AddProperty(const std::string& property, const std::string& value)
  {
    if(StringUtils::IsWord(property))
      properties.emplace(property, value);
    else
      Log::Error("XML property name can only be made up of letters");
  
  }

  XMLObject XMLObject::GetStrippedXMLObject() const
  {
    if(text == "")
      return XMLObject(name, properties, objects);
    else
      return XMLObject(name, properties, text);
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

    //  Read all properties of the xml tag
    ReadProperties(string, posPointer, linePointer);

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
      throw XMLException((std::string("Invalid character proceeding properties in opening XML Tag \"") + string[*posPointer] + "\".").c_str(), *linePointer);
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

  void XMLObject::ReadProperties(const std::string& string, int* posPointer, int* linePointer)
  {
    ReadWhiteSpace(string, posPointer, linePointer);

    while (string[*posPointer] != '>' && string[*posPointer] != '/')
    {
      ReadProperty(string, posPointer, linePointer);
    }
  }

  void XMLObject::ReadProperty(const std::string& string, int* posPointer, int* linePointer)
  {

    std::string property = StringUtils::GetWord(string, *posPointer);
    if (property.length() == 0)
      throw XMLException((std::string("Invalid character proceeding name \"") + string[*posPointer] + "\".").c_str(), *linePointer);
    if (properties.count(property) > 0)
      throw XMLException((std::string("Duplicate property in XML tag \"") + property + "\".").c_str(), *linePointer);
    *posPointer += property.length();
    ReadWhiteSpace(string, posPointer, linePointer);
    if (string[*posPointer] != '=')
      throw XMLException((std::string("Invalid character proceeding property name in XML Tag \"") + string[*posPointer] + "\".").c_str(), *linePointer);
    (*posPointer)++;
    ReadWhiteSpace(string, posPointer, linePointer);
    if (string[*posPointer] != '\"')
      throw XMLException("XML property value is not inside enclosing quotes.", *linePointer);
    (*posPointer)++;
    int valueStart = *posPointer;
    while (string[*posPointer] != '\"' || string[*posPointer - 1] == '\\') (*posPointer)++;
    std::string value = string.substr(valueStart, (*posPointer) - valueStart);
    StringUtils::replace_all(value, "\\\"", "\"");
    (*posPointer)++;
    properties.emplace(property, value);
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
    while (string[*posPointer] != '<' || string[(*posPointer) - 1] == '\\') (*posPointer)++;
    text = string.substr(startPos, (*posPointer) - startPos);
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
}
