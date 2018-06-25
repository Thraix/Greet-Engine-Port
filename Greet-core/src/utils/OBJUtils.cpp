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

  void OBJUtils::ProcessVertex(const std::vector<std::string>& vertexData, std::vector<uint>& indices, const std::vector<Vec2>& texCoords, const std::vector<Vec3>& normals, Vec2* texCoordsArray, Vec3* normalsArray)
  {
    uint currentVertex = atoi(vertexData[0].c_str()) - 1;

    indices.push_back(currentVertex);
    if(vertexData[1].length() == 0)
    {
      if(texCoords.size() != 0)
      {
        texCoordsArray[currentVertex] = Vec2(0,0);
      }
    }
    else
    {
      uint texPos = atoi(vertexData[1].c_str());
      texPos--;
      Vec2 currentTex = texCoords[texPos];
      texCoordsArray[currentVertex] = currentTex;
    }
    if(vertexData[2].length() == 0)
    {
      if(normals.size() != 0)
      {
        normalsArray[currentVertex] = Vec3(0,0,0);
      }
    }
    else
    {
      uint normalPos = atoi(vertexData[2].c_str()) - 1;
      Vec3 currentNormal = normals[normalPos];
      normalsArray[currentVertex] = currentNormal;
    }
  }

  MeshData* OBJUtils::LoadObj(const std::string& filename)
  {
    std::vector<std::string> dataLine;

    std::vector<Vec3> vertices;
    std::vector<Vec2> texCoords;
    std::vector<Vec3> normals;
    std::vector<uint> indices;
    bool atIndex = false;

    Vec2* texCoordsArray = NULL;
    Vec3* normalsArray = NULL;

    std::ifstream input(filename);
    for (std::string line; getline(input, line); )
    {
      dataLine = StringUtils::split_string(line, " ");
      if (dataLine[0].compare("v") == 0)
      {
        const char* pos1 = strchr(line.c_str(), ' ')+1;
        const char* pos2 = strchr(pos1, ' ')+1;
        const char* pos3 = strchr(pos2, ' ')+1;
        Vec3 vector = Vec3(atof(pos1),atof(pos2),atof(pos3));
        vertices.push_back(vector);
      }
      else if (dataLine[0].compare("vt") == 0)
      {
        const char* pos1 = strchr(line.c_str(), ' ')+1;
        const char* pos2 = strchr(pos1, ' ')+1;
        Vec2 texCoord = Vec2(atof(pos1),atof(pos2));
        texCoords.push_back(texCoord);
      }
      else if (dataLine[0].compare("vn") == 0)
      {
        const char* pos1 = strchr(line.c_str(), ' ')+1;
        const char* pos2 = strchr(pos1, ' ')+1;
        const char* pos3 = strchr(pos2, ' ')+1;
        Vec3 normal = Vec3(atof(pos1),atof(pos2),atof(pos3));
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

        ProcessVertex(vertex1, indices, texCoords, normals, texCoordsArray, normalsArray);
        ProcessVertex(vertex2, indices, texCoords, normals, texCoordsArray, normalsArray);
        ProcessVertex(vertex3, indices, texCoords, normals, texCoordsArray, normalsArray);
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

    MeshData* mesh = new MeshData(verticesArray, vertices.size(), indicesArray, indices.size());
    if(normals.size() > 0)
      mesh->AddAttribute(new AttributeData<Vec3>(ATTRIBUTE_NORMAL, normalsArray));
    if(texCoords.size() > 0)
      mesh->AddAttribute(new AttributeData<Vec2>(ATTRIBUTE_TEXCOORD, texCoordsArray));
    return mesh;
  }
}
