#pragma once

#include <vector>
#include <graphics/models/Mesh.h>
#include <string>
#include <fstream>

namespace Greet { 
  class OBJUtils
  {
    private:
      static inline MeshData* ErrorModel();
      static inline void ProcessVertex(const std::vector<std::string>& vertexData, std::vector<uint>& indices, std::vector<uint>& indicesArray, const std::vector<Vec2>& texCoords, const std::vector<Vec3>& normals, Vec2* texCoordsArray, Vec3* normalsArray);
      static inline void ReadVec2s(FILE* file, Vec2* Vec2s, uint amount);
      static inline void ReadVec3s(FILE* file, Vec3* Vec3s, uint amount);
      static inline void ReadUints(FILE* file, uint* uints, uint count, Vec3* normalsArray, Vec2* texCoordsArray, Vec3* normals, Vec2* texCoords);

    public:
      static MeshData* ConvertToGobj(const std::string& filename);
      static MeshData* LoadObj(const std::string& filename);
      static MeshData* LoadObjAsLowPoly(const std::string& filename);
  };
}
