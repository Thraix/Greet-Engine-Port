#if 0
#include "JSONArrayObject.h"

#include "JSONArray.h"
#include "JSONObject.h"

/* *
	Not currently supported
*/
namespace Greet {

	JSONArrayObject::JSONArrayObject(std::vector<std::string> values, std::vector<JSONObject> objects, std::vector<JSONArray> arrays)
		: m_values(values), m_objects(objects), m_arrays(arrays)
	{
	
	}

	JSONArrayObject::JSONArrayObject(std::string* values, uint valueSize, JSONObject* objects, uint objectSize, JSONArray* arrays, uint arraySize)
	{
		m_values = std::vector<std::string>(values,values+valueSize);
		m_objects = std::vector<JSONObject>(objects, objects + objectSize);
		m_arrays = std::vector<JSONArray>(arrays,arrays+arraySize);
	}
}
#endif
