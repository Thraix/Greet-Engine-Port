#include "OBJUtils.h"

#include <utils/FileUtils.h>
#include <utils/StringUtils.h>
#include <graphics/models/MeshFactory.h>

#define GOBJ_VECTOR3_SIZE sizeof(float) * 3 // 13
#define GOBJ_VECTOR2_SIZE sizeof(float) * 2 // 9
#define GOBJ_UINT_SIZE sizeof(uint)			// 5
#define GOBJ_BUFFER_SIZE 1024

namespace Greet {

  MeshData* OBJUtils::ErrorModel()
  {
    return MeshFactory::Cube(0,0,0,1,1,1);// TODO return cube
  }

  MeshData* OBJUtils::LoadObj(const std::string& filename)
  {
    std::vector<std::string> dataLine;

    std::vector<Vec3<float>> positions;
    std::vector<Vec2> texCoords;
    std::vector<Vec3<float>> normals;
    std::vector<uint> indices;

    std::ifstream input(filename);
    std::map<std::string, uint> verticesMap;

    // These are the same length.
    std::vector<Vec3<float>> vertexPos;
    std::vector<Vec2> vertexTexCoords;
    std::vector<Vec3<float>> vertexNormals;

    for (std::string line; getline(input, line); )
    {
      std::istringstream iss{line};
      std::string s;
      iss >> s;
      if (s == "v")
      {
        Vec3<float> vector{};
        iss >> vector.x;
        iss >> vector.y;
        iss >> vector.z;
        positions.push_back(vector);
      }
      else if (s == "vt")
      {
        Vec2 vector{};
        iss >> vector.x;
        iss >> vector.y;
        texCoords.push_back(vector);
      }
      else if (s == "vn")
      {
        Vec3<float> vector{};
        iss >> vector.x;
        iss >> vector.y;
        iss >> vector.z;
        normals.push_back(vector);
      }
      else if (s == "f")
      {
        dataLine = StringUtils::split_string(line, " ");
        for(int i = 0; i<3;i++)
        {
          auto it = verticesMap.find(dataLine[1 + i]);
          if(it != verticesMap.end())
          {
            indices.push_back(it->second);
          }
          else
          {
            indices.push_back(vertexPos.size());
            verticesMap.emplace(dataLine[i + 1], vertexPos.size());
            std::vector<std::string> vertex = StringUtils::split_string(dataLine[i + 1], "/");

            // Only position defined
            if(vertex.size() == 1)
            {
              vertexPos.push_back(positions[atoi(vertex[0].c_str())-1]);
            }
            else if(vertex.size() == 2) // Position and texcoord defined
            {
              vertexPos.push_back(positions[atoi(vertex[0].c_str())-1]);
              if(vertex[1] != "")
                vertexTexCoords.push_back(texCoords[atoi(vertex[1].c_str())-1]);
            }
            else if(vertex.size() == 3)
            {
              vertexPos.push_back(positions[atoi(vertex[0].c_str())-1]);
              if(vertex[1] != "")
                vertexTexCoords.push_back(texCoords[atoi(vertex[1].c_str())-1]);
              if(vertex[2] != "")
                vertexNormals.push_back(normals[atoi(vertex[2].c_str())-1]);
            }
          }
        }
      }
    }

    input.close();
    Vec3<float>* positionsArray = new Vec3<float>[vertexPos.size()];
    Vec3<float>* normalsArray = new Vec3<float>[vertexNormals.size()];
    Vec2* texCoordsArray = new Vec2[vertexTexCoords.size()];
    uint* indicesArray = new uint[indices.size()];
    memcpy(positionsArray, vertexPos.data(), vertexPos.size() * sizeof(Vec3<float>));
    memcpy(texCoordsArray, vertexTexCoords.data(), vertexTexCoords.size() * sizeof(Vec2));
    memcpy(normalsArray, vertexNormals.data(), vertexNormals.size() * sizeof(Vec3<float>));
    memcpy(indicesArray, indices.data(), indices.size() * sizeof(uint));

    MeshData* mesh = new MeshData(positionsArray, vertexPos.size(), indicesArray, indices.size());
    if(vertexTexCoords.size() > 0)
      mesh->AddAttribute(new AttributeData<Vec2>(ATTRIBUTE_TEXCOORD, texCoordsArray));
    else
      delete[] texCoordsArray;

    if(vertexNormals.size() > 0)
      mesh->AddAttribute(new AttributeData<Vec3<float>>(ATTRIBUTE_NORMAL, normalsArray));
    else
      delete[] normalsArray;

    return mesh;
  }
}
