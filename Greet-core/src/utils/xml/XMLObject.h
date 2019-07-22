#pragma once

#include <string>
#include <map>
#include <vector>
namespace Greet
{
  class XMLObject
  {
    private:
      std::string name;
      std::string text;

      std::map<std::string, std::string> attributes;
      std::vector<XMLObject> objects;

    public:
      XMLObject() {}
      XMLObject(const std::string& string);
      XMLObject(const std::string& string, int* pos, int* line);
      XMLObject(const std::string& name, const std::map<std::string, std::string>& properties, const std::string& text);
      XMLObject(const std::string& name, const std::map<std::string, std::string>& properties, const std::vector<XMLObject>& objects);

      bool HasAttribute(const std::string& property) const;
      const std::string& GetAttribute(const std::string& property) const;
      const std::string& GetAttribute(const std::string& property, const std::string& defaultValue) const;

      unsigned int GetObjectCount() const;
      const XMLObject& GetObject(unsigned int i) const;
      const std::vector<XMLObject>& GetObjects() const;
      const std::string& GetName() const;
      const std::string& GetText() const;
      XMLObject GetStrippedXMLObject() const;

      void SetName(const std::string& name);
      void SetText(const std::string& text);
      void AddAttribute(const std::string& property, const std::string& value);

    private:
      std::string GetClosingTag(const std::string& string, int* posPointer, int* linePointer);
      // Returns true if the head contained closing tag.
      bool ReadHead(const std::string& string, int* posPointer, int* linePointer);
      void ReadName(const std::string& string, int* posPointer, int* linePointer);
      void ReadAttribute(const std::string& string, int* posPointer, int* linePointer);
      void ReadAttributes(const std::string& string, int* posPointer, int* linePointer);
      void ReadBodyTail(const std::string& string, int* posPointer, int* linePointer);
      void ReadText(const std::string& string, int* posPointer, int* linePointer);
      void ReadWhiteSpace(const std::string& string, int* posPointer, int* linePointer);
      void ReplacePredefinedEntities(std::string& string);
      std::string ReadXMLName(const std::string& string, int* posPointer, int* linePointer); 
  };
}
