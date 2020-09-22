#pragma once

#include <ecs/NativeScript.h>
#include <ecs/ECSComponents.h>
#include <graphics/models/MeshFactory.h>
#include <utils/Noise.h>

class TerrainGenerationScript : public Greet::NativeScript
{
  public:
    TerrainGenerationScript(const Greet::Entity& entity)
      : NativeScript{entity}
    {}

    void Create() override
    {
      ASSERT(entity.HasComponent<Greet::MeshComponent>(), "Terrain does not contain meshcomponent");
      Greet::MeshComponent& mesh = entity.GetComponent<Greet::MeshComponent>();
      Greet::Pointer<float> noise = Greet::Noise::GenNoise(100, 100, 5, 8, 8, 0.5f);
      ProcessNoise(noise);
      Greet::MeshData data{Greet::MeshFactory::Grid({99, 99}, noise, {0.0f, 0.0f, 0.0f}, {99.0f, 1.0f, 99.0f})};
      mesh.mesh.reset(new Greet::Mesh{data});
    }

    void ProcessNoise(Greet::Pointer<float>& noise)
    {
      for(float& f : noise)
      {
        if (f < 0.45)
        {
          //y = 0.45f + Noise::PRNG(vertex->x, vertex->z)*0.01f;// + 0.03f*(rand() / (float)RAND_MAX - 0.5f);
        }
        else if (f < 0.48)
        {

        }
        else if (f < 0.58)
        {

        }
        else if (f < 0.65)
        {
          f = (pow(f - 0.58, 0.6) + 0.58);
        }
        else
        {
          f = (pow(f - 0.58, 0.6) + 0.58);
        }
        f *= 20;
      }
    }
};
