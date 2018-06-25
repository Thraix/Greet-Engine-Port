#include "OBJUtils.h"

#include <utils/FileUtils.h>
#include <utils/StringUtils.h>

#define GOBJ_VECTOR3_SIZE sizeof(float) * 3 // 13
#define GOBJ_VECTOR2_SIZE sizeof(float) * 2 // 9
#define GOBJ_UINT_SIZE sizeof(uint)			// 5
#define GOBJ_BUFFER_SIZE 1024

namespace Greet {


	Mesh* OBJUtils::ErrorModel()
	{
		return NULL;// TODO return cube
	}

	Vec3 OBJUtils::LoadVector3(std::vector<std::string> dataLine)
	{
		return Vec3(atof(dataLine[1].c_str()), atof(dataLine[2].c_str()), atof(dataLine[3].c_str()));
	}

	Vec2 OBJUtils::LoadVector2(std::vector<std::string> dataLine)
	{
		return Vec2(atof(dataLine[1].c_str()), atof(dataLine[2].c_str()));
	}

	void OBJUtils::ProcessVertex(const std::vector<std::string>& vertexData, std::vector<uint>& indices, std::vector<uint>& indicesArray, const std::vector<Vec2>& texCoords, const std::vector<Vec3>& normals, Vec2* texCoordsArray, Vec3* normalsArray)
	{
		uint currentVertex = atoi(vertexData[0].c_str()) - 1;

		indices.push_back(currentVertex);
		indicesArray.push_back(currentVertex);
		uint texPos = atoi(vertexData[1].c_str());
    if(texPos == 0)
    {

      indicesArray.push_back(0);
      texCoordsArray[currentVertex] = Vec2(0,0);
    }
    else
    {
      texPos--;
      Vec2 currentTex = texCoords[texPos];
      indicesArray.push_back(texPos);
      texCoordsArray[currentVertex] = currentTex;
    }
		uint normalPos = atoi(vertexData[2].c_str()) - 1;
		Vec3 currentNormal = normals[normalPos];
		indicesArray.push_back(normalPos);
		normalsArray[currentVertex] = currentNormal;
	}

	Mesh* OBJUtils::ConvertToGobj(const std::string& filename)
	{
		std::vector<std::string> dataLine;

		std::vector<Vec3> vertices;
		std::vector<Vec2> texCoords;
		std::vector<Vec3> normals;
		std::vector<uint> indices;
		std::vector<uint> indicesArray2;
		bool atIndex = false;

		Vec2* texCoordsArray = NULL;
		Vec3* normalsArray = NULL;

		std::ifstream input(filename);
		for (std::string line; getline(input, line); )
		{
			dataLine = StringUtils::split_string(line, " ");
			if (dataLine[0].compare("v") == 0)
			{
				Vec3 vector = LoadVector3(dataLine);
				vertices.push_back(vector);
			}
			else if (dataLine[0].compare("vt") == 0)
			{
				Vec2 texCoord = LoadVector2(dataLine);
				texCoords.push_back(texCoord);
			}
			else if (dataLine[0].compare("vn") == 0)
			{
				Vec3 normal = LoadVector3(dataLine);
				normals.push_back(normal);
			}
			else if (dataLine[0].compare("f") == 0)
			{
				if (!atIndex)
				{
					texCoordsArray = new Vec2[vertices.size()];
					normalsArray = new Vec3[vertices.size()];
					atIndex = true;
				}
				std::vector<std::string> vertex1 = StringUtils::split_string(dataLine[1], "/");
				std::vector<std::string> vertex2 = StringUtils::split_string(dataLine[2], "/");
				std::vector<std::string> vertex3 = StringUtils::split_string(dataLine[3], "/");

				ProcessVertex(vertex1, indices, indicesArray2, texCoords, normals, texCoordsArray, normalsArray);
				ProcessVertex(vertex2, indices, indicesArray2, texCoords, normals, texCoordsArray, normalsArray);
				ProcessVertex(vertex3, indices, indicesArray2, texCoords, normals, texCoordsArray, normalsArray);
			}
		}

		input.close();
		Vec3* verticesArray = new Vec3[vertices.size()];
		uint* indicesArray = new uint[indices.size()];
		int i = 0;
		for (Vec3 vertex : vertices)
		{
			verticesArray[i++] = vertex;

		}
		for (uint i = 0;i < indices.size();i++)
		{
			indicesArray[i] = indices[i];
		}

		FILE *file = fopen((filename + ".gobj").c_str(), "wb+");
		if (!file)
		{
			Log::Error("Obj could not be read: ", filename);
			return ErrorModel(); // TODO return cube model
		}

		fwrite("GOBJ",4,1,file);
		uint v = vertices.size();
		uint t = texCoords.size();
		uint n = normals.size();
		uint in = indices.size();
		fwrite(&v, sizeof(uint), 1, file);
		fwrite(&t, sizeof(uint), 1, file);
		fwrite(&n, sizeof(uint), 1, file);
		fwrite(&in, sizeof(uint), 1, file);
		uint j = 0;
		for (Vec3 vertex : vertices)
		{
			fwrite(&vertex.x, sizeof(float), 1, file);
			fwrite(&vertex.y, sizeof(float), 1, file);
			fwrite(&vertex.z, sizeof(float), 1, file);
			j++;
		}

		for (Vec2 texCoord : texCoords)
		{
			fwrite(&texCoord.x, sizeof(float), 1, file);
			fwrite(&texCoord.y, sizeof(float), 1, file);
		}

		for (Vec3 normal : normals)
		{
			fwrite(&normal.x, sizeof(float), 1, file);
			fwrite(&normal.y, sizeof(float), 1, file);
			fwrite(&normal.z, sizeof(float), 1, file);
		}
		for (uint j : indicesArray2)
		{
			fwrite(&j, 1, sizeof(uint), file);
		}
		fwrite("GOBJ", 1, 4, file);
		fclose(file);
		Mesh* mesh = new Mesh(verticesArray, vertices.size(), indicesArray, indices.size());
		mesh->AddAttribute(MESH_NORMALS_LOCATION, normalsArray);
		mesh->AddAttribute(MESH_TEXCOORDS_LOCATION, texCoordsArray);
		return mesh;
	}

	void OBJUtils::ReadVec2s(FILE* file, Vec2* Vec2s, uint amount)
	{
		Vec2* data = new Vec2[amount + 1];
		fread((float*)data, sizeof(float), amount * 2, file);
		for (uint i = 0;i < amount;i++)
		{
			Vec2s[i] = data[i];
		}
		delete[] data;
	}

	void OBJUtils::ReadVec3s(FILE* file, Vec3* Vec3s, uint amount)
	{
		Vec3* data = new Vec3[amount + 1];
		fread((float*)data, sizeof(float), amount * 3, file);
		for (uint i = 0;i < amount;i++)
		{
			Vec3s[i] = data[i];
		}
		delete[] data;
	}

	void OBJUtils::ReadUints(FILE* file, uint* uints, uint count, Vec3* normalsArray, Vec2* texCoordsArray, Vec3* normals, Vec2* texCoords)
	{
		uint* data = new uint[count*3+1];
		fread(data, sizeof(uint), count * 3, file);
		uint i = 0;
		for (uint j = 0;j < count * 3;j += 3,i++)
		{
			uint vertex = data[j];
			uint texCoord = data[j + 1];
			uint normal = data[j + 2];
			uints[i] = vertex;
			texCoordsArray[vertex] = texCoords[texCoord];
			normalsArray[vertex] = normals[normal];
		}
		memset(data, 0xffffffff, count * 3 * sizeof(uint));
		delete[] data;
	}

	Mesh* OBJUtils::LoadObj(const std::string& filename)
	{

		FILE *file = fopen(filename.c_str(), "rb");
		if (!file)
		{
			Log::Error("Obj could not be read: ", filename);
			ErrorHandle::SetErrorCode(GREET_ERROR_GOBJ_READ);
			return ErrorModel();
		}

		char* data = new char[5];
		fseek(file, 0, SEEK_END);
		ulong length = ftell(file);
		fseek(file, 0, SEEK_SET);
		if (length < sizeof(char) * 4 + sizeof(uint) * 4)
		{
			fclose(file);
			Log::Error("File format not supported, if you are using obj you need to compile it to gobj: ", filename);
			ErrorHandle::SetErrorCode(GREET_ERROR_GOBJ_FORMAT);
			return ErrorModel();
		}
		data[4] = '\0';
		fread(data, sizeof(char), 4, file);
		if (std::string(data).compare("GOBJ") != 0)
		{
			fclose(file);
			Log::Error("File format not supported, if you are using obj you need to compile it to gobj: ",filename);
			ErrorHandle::SetErrorCode(GREET_ERROR_GOBJ_FORMAT);
			return ErrorModel();
		}
		delete[] data;

		uint* uintData = new uint[4 + 1];
		fread(uintData, sizeof(uint), 4, file);
		uint vertexCount = uintData[0];
		uint texCoordCount = uintData[1];
		uint normalCount = uintData[2];
		uint indexCount = uintData[3];
		delete[] uintData;
		if (length < (vertexCount * 3 + texCoordCount * 2 + normalCount * 3) * sizeof(float) + indexCount * sizeof(uint))
		{
			fclose(file);
			Log::Error("GOBJ file could not be read: ",filename);
			ErrorHandle::SetErrorCode(GREET_ERROR_GOBJ_READ);
			return ErrorModel();
		}
		Vec3* vertices = new Vec3[vertexCount];
		Vec2* texCoords = new Vec2[texCoordCount];
		Vec3* normals = new Vec3[normalCount];
		uint* indices = new uint[indexCount];

		Vec2* texCoordsArray = new Vec2[vertexCount];
		Vec3* normalsArray = new Vec3[vertexCount];

    // asdasdasdad
		ReadVec3s(file, vertices, vertexCount);
		ReadVec2s(file, texCoords, texCoordCount);
		ReadVec3s(file, normals, normalCount);
		ReadUints(file, indices, indexCount, normalsArray, texCoordsArray, normals, texCoords);
		fclose(file);
		Mesh* mesh = new Mesh(vertices, vertexCount, indices, indexCount);
		mesh->AddAttribute(MESH_NORMALS_LOCATION, normalsArray); 
		mesh->AddAttribute(MESH_TEXCOORDS_LOCATION, texCoordsArray);
		return mesh;
	}

  MeshData* OBJUtils::LoadObjAsLowPoly(const std::string& filename)
  {
    filename.length();
    return NULL;
  }

}
