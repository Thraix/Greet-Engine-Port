#pragma once

#include <string>
#include <map>
#include <vector>
namespace Greet
{
	class XMLObject
	{
	private:
		// Used for reading the xml file.
		int dataLength;

		std::string name;
		std::string text;

		std::map<std::string, std::string> properties;
		std::vector<XMLObject> objects;

	public:
		XMLObject() {}
		XMLObject(const std::string& string);
		XMLObject(const std::string& string, int* pos, int* line);
		XMLObject(const std::string& name, const std::map<std::string, std::string> properties, const std::string& text);
		XMLObject(const std::string& name, const std::map<std::string, std::string> properties, const std::vector<XMLObject> objects);

		bool HasProperty(const std::string& property) const;
		const std::string& GetProperty(const std::string& property) const;
		unsigned int GetObjectCount() const;
		const XMLObject& GetObject(unsigned int i) const;
		const std::string& GetName() const;
		const std::string& GetText() const;
		XMLObject GetStrippedXMLObject() const;

	private:
		std::string GetClosingTag(const std::string& string, int* posPointer, int* linePointer);
		// Returns true if the head contained closing tag.
		bool ReadHead(const std::string& string, int* posPointer, int* linePointer);
		void ReadName(const std::string& string, int* posPointer, int* linePointer);
		void ReadProperty(const std::string& string, int* posPointer, int* linePointer);
		void ReadProperties(const std::string& string, int* posPointer, int* linePointer);
		void ReadBodyTail(const std::string& string, int* posPointer, int* linePointer);
		void ReadText(const std::string& string, int* posPointer, int* linePointer);
		void ReadWhiteSpace(const std::string& string, int* posPointer, int* linePointer);
	};
}