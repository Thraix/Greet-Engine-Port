#pragma once
#include <exception>
namespace Greet
{
	class XMLException : public std::exception
	{
	private:
		std::string m_message;
	public:
		explicit XMLException(const std::string& message) : m_message("XMLException: " + message) {}
		explicit XMLException(const std::string& message, int line) : m_message(("XMLException: " + message + " at line " + std::to_string(line))) {}

		virtual const char* what() const throw()
		{
			return m_message.c_str();
		}
	};
}