#include "Chunk.h"

#include <graphics/models/MeshFactory.h> 

using namespace Greet;

uint Chunk::CHUNK_WIDTH = 32;
uint Chunk::CHUNK_HEIGHT = 32;

Chunk::Chunk()
{}

void Chunk::Initialize(uint posX, uint posY)
{
  this->posX = posX;
  this->posY = posY;
  float* noise = Noise::GenNoise(CHUNK_WIDTH+1, CHUNK_HEIGHT+1,4,16, 16,0.75f, posX * CHUNK_WIDTH, posY * CHUNK_HEIGHT);
  MeshData* data = MeshFactory::LowPolyGrid(0,0,0,CHUNK_WIDTH, CHUNK_HEIGHT,CHUNK_WIDTH, CHUNK_HEIGHT,noise, 1.0f);
  RecalcGrid(data);
  mesh = new Mesh(data);
  delete data;
  delete[] noise;
}

Chunk::~Chunk()
{
  delete mesh;
}

void Chunk::RecalcPositions(Vec3<float>& vertex)
{
  float y = vertex.y;
  if (y < 0.45)
  {
    //y = 0.45f + Noise::PRNG(vertex->x, vertex->z)*0.01f;// + 0.03f*(rand() / (float)RAND_MAX - 0.5f);
  }
  else if (y < 0.48)
  {

  }
  else if (y < 0.58)
  {

  }
  else if (y < 0.65)
  {
    y = (pow(y - 0.58, 0.6) + 0.58);
  }
  else
  {
    y = (pow(y - 0.58, 0.6) + 0.58);
  }
  vertex.y = y * 20.0;
}

void RecalcColors(const Vec3<float>& v1, const Vec3<float>& v2, const Vec3<float>& v3, uint* color)
{
  float y = (v1.y + v2.y + v3.y) / 3.0f / 20.0f;
  if (y < 0.45+0.01f)
  {
    uint blue = (uint)(pow(1, 4.0f) * 255);
    blue = blue > 255 ? 255 : blue;
    *color = 0xff000000 | ((blue / 2) << 16) | ((uint)(blue * 0.9) << 8) | blue;
    *color = 0xffF0E5A5;
  }
  else if (y < 0.48)
  {
    *color = 0xffF0E5A5;
  }
  else if (y < 0.58)
  {
    *color = 0xff7CD663;
  }
  else if (y < pow(0.07, 0.6) + 0.58)
  {
    *color = 0xffB5B0A8;
  }
  else
  {
    *color = 0xffDCF2F2;
  }
}

void Chunk::CalcGridVertexOffset(MeshData* data)
{
  const std::vector<Vec3<float>>& vertices = data->GetVertices();
  const std::vector<uint>& indices = data->GetIndices();

  std::vector<byte> offsets = std::vector<byte>(4 * vertices.size());
  for (int i = 0;i < indices.size();i+=3)
  {
    Vec3<float> v1 = vertices[indices[i+1]] - vertices[indices[i]];
    Vec3<float> v2 = vertices[indices[i+2]] - vertices[indices[i]];
    offsets[indices[i]*4] = round(v1.x);
    offsets[indices[i]*4 + 1] = round(v1.z);
    offsets[indices[i]*4 + 2] = round(v2.x);
    offsets[indices[i]*4 + 3] = round(v2.z);
  }
  data->AddAttribute(new AttributeData(AttributeDefaults(4, 4, 4 * sizeof(byte), GL_BYTE,GL_FALSE), offsets));
}

void Chunk::RecalcGrid(MeshData* data)
{
  std::vector<uint> colors = std::vector<uint>(data->GetVertexCount());
  std::vector<Vec3<float>>& vertices = data->GetVertices();
  std::vector<uint>& indices = data->GetIndices();
  Vec3<float>* normals = (Vec3<float>*)data->GetAttribute(ATTRIBUTE_NORMAL)->data.data();

  for (int i = 0;i < indices.size();i+=3)
  {
    RecalcPositions(vertices[indices[i]]);
  }
  uint index = MeshFactory::IndexGrid(CHUNK_WIDTH, CHUNK_HEIGHT - 1, CHUNK_WIDTH, CHUNK_HEIGHT);
  RecalcPositions(vertices[MeshFactory::IndexGrid(CHUNK_WIDTH,CHUNK_HEIGHT-1,CHUNK_WIDTH,CHUNK_HEIGHT)]);
  index = MeshFactory::IndexGrid(0, CHUNK_HEIGHT, CHUNK_WIDTH, CHUNK_HEIGHT);
  RecalcPositions(vertices[index]);
  for (int i = 0;i < indices.size();i += 3)
  {
    normals[indices[i]] = MeshFactory::CalculateNormal(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]);
    RecalcColors(vertices[indices[i]], vertices[indices[i+1]], vertices[indices[i+2]], &colors[indices[i]]);
  }
  data->AddAttribute(new AttributeData(ATTRIBUTE_COLOR, colors));
}
