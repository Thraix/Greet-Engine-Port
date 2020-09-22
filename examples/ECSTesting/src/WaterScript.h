#pragma once

#include <ecs/NativeScript.h>
#include <ecs/ECSComponents.h>
#include <graphics/models/MeshFactory.h>
#include <utils/Noise.h>

class WaterScript : public Greet::NativeScript
{
  private:
    float time;
  public:

    void Create() override
    {
      ASSERT(entity.HasComponent<Greet::MaterialComponent>(), "Water does not contain MaterialComponent");
      Greet::MaterialComponent& material = entity.GetComponent<Greet::MaterialComponent>();
      material.material->GetShader()->Enable();
      material.material->SetSpecularExponent(50);
      material.material->SetSpecularStrength(0.4);
      material.material->GetShader()->Disable();
    }

    void Update(float timeElapsed) override
    {
      time += timeElapsed;
      ASSERT(entity.HasComponent<Greet::MaterialComponent>(), "Water does not contain MaterialComponent");
      Greet::MaterialComponent& material = entity.GetComponent<Greet::MaterialComponent>();
      material.material->GetShader()->Enable();
      material.material->GetShader()->SetUniform1f("uTime", time);
      material.material->GetShader()->Disable();
    }
};
