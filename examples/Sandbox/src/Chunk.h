#pragma once

#include <graphics/models/Mesh.h>
#include <utils/Noise.h>

class Chunk
{
  friend class World;
  private:
  static uint CHUNK_WIDTH;
  static uint CHUNK_HEIGHT;

  Greet::Mesh* mesh;
  uint posX;
  uint posY;
  private:
  Chunk();
  void Initialize(uint posX, uint posY);

  void RecalcPositions(Greet::Vec3f& vertex);
  void CalcGridVertexOffset(Greet::MeshData* data);
  void RecalcGrid(Greet::MeshData& data);

  public:
    virtual ~Chunk();


};
