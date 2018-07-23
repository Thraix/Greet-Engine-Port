#pragma once

#include <utils/json/JSONArray.h>
#include <internal/GreetTypes.h>
#include <map>

namespace Greet {

	class JSONObject
	{
		friend JSONArray;

	private:
		// Static Null values
		static std::string valueNull;
		static JSONObject objectNull;
		static JSONArray arrayNull;

		std::map<std::string, std::string> m_values;
		std::map<std::string, JSONArray> m_arrays;
		std::map<std::string, JSONObject> m_objects;

	public:
		JSONObject();
		JSONObject(const JSONObject& object);
		JSONObject(std::map<std::string,std::string> values,std::map<std::string,JSONObject> objects,std::map<std::string,JSONArray> arrays);

		bool HasKey(const std::string& key);

		void AddValue(std::string key, std::string value);
		void AddArray(std::string key, JSONArray value);
		void AddObject(std::string key, JSONObject value);

		const std::string& GetValue(const std::string& key) const;
		uint GetValueAsUint(const std::string& key) const;
		int GetValueAsInt(const std::string& key) const;
		float GetValueAsFloat(const std::string& key) const;
		bool GetValueAsBool(const std::string& key) const;
		bool IsNull(const std::string& key) const;

		const JSONArray& GetArray(const std::string& key) const;
		const JSONObject& GetObject(const std::string& key) const;

		friend std::ostream& operator<<(std::ostream& os, const JSONObject& obj);
	};

}
