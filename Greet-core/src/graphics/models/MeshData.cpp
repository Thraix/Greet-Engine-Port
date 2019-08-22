#include "MeshData.h"
#include <logging/Log.h>
#include <fstream>
#include <graphics/models/MeshFactory.h>
#include <set>
#include <map>

namespace Greet {

  MeshData::MeshData(const std::vector<Vec3<float>>& vertices, const std::vector<uint>& indices)
    : m_vertices(vertices), m_indices(indices)
  {
    for(auto it = m_indices.begin(); it != m_indices.end(); ++it)
    {
      if(*it >= m_vertices.size())
      {
        Log::Error("Index out of bound in MeshData creation");
        *it = 0;
      }
    }
  }

  MeshData::MeshData(std::vector<Vec3<float>>&& vertices, std::vector<uint>&& indices)
    : m_vertices{std::move(vertices)},m_indices{std::move(indices)}
  {
    for(auto it = m_indices.begin(); it != m_indices.end(); ++it)
    {
      if(*it >= m_vertices.size())
      {
        Log::Error("Index out of bound in MeshData creation");
        *it = 0;
      }
    }
  }

  MeshData::~MeshData()
  {
  }

  void MeshData::AddAttribute(AttributeData&& data)
  {
    if(data.data.size() / data.memoryValueSize != m_vertices.size())
    {
      Log::Error("Attribute length doesn't match vertices length");
      Log::Error("Attribute Length: ", data.data.size() / data.memoryValueSize);
      Log::Error("Vertices Length:  ", m_vertices.size());
      ASSERT(false, "");
    }
    else
      m_data.push_back(data);
  }

  const AttributeData* MeshData::GetAttribute(AttributeDefaults defaults) const
  {
    for (auto it = m_data.begin(); it != m_data.end(); it++)
    {
      if ((*it).location == defaults.location)
      {
        return &*it;
      }
    }
    return nullptr;
  }

  AttributeData* MeshData::RemoveAttribute(AttributeDefaults defaults)
  {
    for (auto it = m_data.begin(); it != m_data.end(); it++)
    {
      if ((*it).location == defaults.location)
      {
        m_data.erase(it);
        return &*it;
      }
    }
    return nullptr;
  }

  void MeshData::GenerateNormals()
  {
    std::vector<Vec3<float>> normals(m_vertices.size());
    MeshFactory::CalculateNormals(m_vertices, m_indices, normals);
    AddAttribute(AttributeData(ATTRIBUTE_NORMAL, normals));
  }

  MeshData* MeshData::LowPolify()
  {
    std::set<uint> usedProvokingVertices; 
    std::map<uint,uint> usedIndices; 
    std::vector<Vec3<float>> newVertices;
    std::vector<uint> newIndices;

    // Loop through all triangles
    for(uint i = 0;i<GetIndexCount();i+=3)
    {
      bool foundProvoking = true;
      uint provokingOffset = 0;

      // Check if the first vertex has been used as a provoking vertex
      if(usedProvokingVertices.count(m_indices[i]) == 0)
      {
        provokingOffset = 0;
      }
      else if(usedProvokingVertices.count(m_indices[i+1]) == 0)
      {
        provokingOffset = 1;
      }
      else if(usedProvokingVertices.count(m_indices[i+2]) == 0)
      {
        provokingOffset = 2;
      }
      else
      {
        foundProvoking = false;
      }

      // First (provoking) vertex
      if(foundProvoking)
      {
        uint provokingIndex = m_indices[i+provokingOffset];
        Vec3<float> provokingVertex = m_vertices[provokingIndex];
        usedProvokingVertices.emplace(provokingIndex);

        auto vertex = usedIndices.find(provokingIndex);
        if(!foundProvoking || vertex == usedIndices.end())
        {
          newIndices.push_back(newVertices.size());
          usedIndices.emplace(provokingIndex, newVertices.size());
          newVertices.push_back(provokingVertex);
        }
        else
        {
          newIndices.push_back(usedIndices.find(provokingIndex)->second);
        }
      }
      else
      {

        newIndices.push_back(newVertices.size());
        newVertices.push_back(m_vertices[m_indices[i]]);
      }

      // Second vertex
      uint index = m_indices[i+((provokingOffset+1)%3)];
      auto vertex = usedIndices.find(index);
      if(vertex == usedIndices.end())
      {
        newIndices.push_back(newVertices.size());
        usedIndices.emplace(index, newVertices.size());
        newVertices.push_back(m_vertices[index]);
      }
      else
      {
        newIndices.push_back(usedIndices.find(index)->second);
      }

      // Third vertex
      index = m_indices[i+((provokingOffset+2)%3)];
      vertex = usedIndices.find(index);
      if(vertex == usedIndices.end())
      {
        newIndices.push_back(newVertices.size());
        usedIndices.emplace(index, newVertices.size());
        newVertices.push_back(m_vertices[index]);
      }
      else
      {
        newIndices.push_back(usedIndices.find(index)->second);
      }
    }

    return new MeshData(newVertices, newIndices);
  }

  void MeshData::WriteToFile(const std::string& filename) const
  {
    std::ofstream fout(filename);

    // Write signature
    fout.write("MESH",4*sizeof(char));

    uint vertexCount = GetVertexCount();
    uint indexCount = GetIndexCount();
    size_t dataCount = m_data.size();

    // Write how many vertices we have 
    fout.write((char*)&vertexCount,sizeof(uint));
    // Write how many indices we have 
    fout.write((char*)&indexCount,sizeof(uint));
    // Write how many attributes we have
    fout.write((char*)&dataCount,sizeof(size_t));

    // Write the different data of the mesh
    for(auto it = m_data.begin();it != m_data.end(); ++it)
    {
      fout.write((char*)&(*it).location,sizeof(uint));
      fout.write((char*)&(*it).vertexValueSize,sizeof(uint));
      fout.write((char*)&(*it).memoryValueSize,sizeof(uint));
      fout.write((char*)&(*it).glType,sizeof(uint));
      fout.write((char*)&(*it).normalized,sizeof(bool));
    }

    // Write all vertex data.
    fout.write((char*)m_vertices.data(), vertexCount * sizeof(Vec3<float>));

    // Write index data.
    fout.write((char*)m_indices.data(), indexCount * sizeof(uint)); 


    // Write all attribute data
    for(auto it = m_data.begin();it != m_data.end(); ++it)
    {
      fout.write((char*)(*it).data.data(), (*it).memoryValueSize * vertexCount);
    }
    fout.close();
  }

  MeshData MeshData::ReadFromFile(const std::string& filename)
  {
    // Small buffer for initial stuffs
    char buffer[1024];
    std::ifstream fin(filename);

    // Determine size of the file and reset to beginning.
    int fileSize = fin.tellg();
    fin.seekg(0,std::ios::end);
    fileSize = (uint)fin.tellg() - fileSize;
    fin.seekg(0,std::ios::beg);

    // Check if the file is big enough to contain signature, vertex count, index count and attribute count
    fileSize -= 4 * sizeof(char) + 2 * sizeof(uint) + sizeof(size_t);
    if(fileSize < 0)
    {
      Log::Error("Could not read MESH file, file is too small to contain signature.");
      return MeshFactory::Cube(0,0,0,1,1,1);
    }

    // Read signature
    fin.read(buffer, 4 * sizeof(char));

    if(std::string(buffer, 4) != "MESH")
    {
      Log::Error("Could not read MESH file, signature invalid.");
      return MeshFactory::Cube(0,0,0,1,1,1);
    }
    // Read how many attributes we have. 
    uint vertexCount;
    fin.read((char*)&vertexCount,sizeof(uint));
    fileSize -= vertexCount * sizeof(Vec3<float>);
    if(fileSize < 0)
    {
      Log::Error("Could not read MESH file, file is too small to contain vertex data");
      return MeshFactory::Cube(0,0,0,1,1,1);
    }

    // Read how many attributes we have. 
    uint indexCount;
    fin.read((char*)&indexCount,sizeof(uint));
    fileSize -= indexCount * sizeof(uint);
    if(fileSize < 0)
    {
      Log::Error("Could not read MESH file, file is too small to contain vertex data");
      return MeshFactory::Cube(0,0,0,1,1,1);
    }

    // Read how many attributes we have. 
    size_t attributeCount;
    fin.read((char*)&attributeCount,sizeof(size_t));

    // Check if the file is big enough to contain attribute parameters.
    int attribLength = (sizeof(uint) * 4 + sizeof(bool));
    int attribsLength = attributeCount * attribLength; 

    // Remove attribLength from fileSize to easier check sizes later.
    fileSize -= attribsLength;

    if(fileSize < 0)
    {
      Log::Error("Could not read MESH file, file is too small to contain attribute parameters");
      return MeshFactory::Cube(0,0,0,1,1,1);
    }

    std::vector<AttributeDefaults> attributeParameters;

    if(attribLength > 1024)
    {
      Log::Error("Could not read MESH file, too many attributes.");
      return MeshFactory::Cube(0,0,0,1,1,1);
    }
    fin.read(buffer,attribsLength);

    const char* pointer = buffer;
    for(size_t i = 0;i<attributeCount;i++)
    {
      // Read uints
      uint location = ((uint*)pointer)[0];
      uint vertexValueSize = ((uint*)pointer)[1];
      uint memoryValueSize = ((uint*)pointer)[2];
      uint glType = ((uint*)pointer)[3];

      // Move pointer
      pointer += sizeof(uint)*4;

      // Read bool
      bool normalized = ((bool*)buffer)[0];

      // Move pointer
      pointer += sizeof(bool);

      attributeParameters.push_back(AttributeDefaults(location,vertexValueSize, memoryValueSize,glType,normalized));
      fileSize -= memoryValueSize * vertexCount;
      if(fileSize < 0)
      {
        Log::Error("Could not read MESH file, file is too small to contain attribute data");
        return MeshFactory::Cube(0,0,0,1,1,1);
      }
    }
    if(fileSize != 0)
      Log::Warning("MESH file is larger than expected. Something might be wrong...");

    // Read vertices
    std::vector<Vec3<float>> vertices = std::vector<Vec3<float>>(vertexCount);
    fin.read((char*)vertices.data(),vertexCount*sizeof(Vec3<float>));

    // Read indices 
    std::vector<uint> indices = std::vector<uint>(indexCount);
    fin.read((char*)indices.data(),indexCount*sizeof(uint));

    MeshData meshData{vertices,indices};

    // Read attribute data
    for(auto it = attributeParameters.begin();it!=attributeParameters.end();++it)
    {
      std::vector<char> data = std::vector<char>(it->memoryValueSize * vertexCount);
      fin.read(data.data(),it->memoryValueSize * vertexCount);
      meshData.AddAttribute(AttributeData(*it, data));
    }

    fin.close();
    return meshData;
  }
}
