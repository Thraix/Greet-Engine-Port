#pragma once

#include <graphics/models/EntityModel.h>
#include <graphics/models/MeshFactory.h>
#include <graphics/textures/TextureManager.h>
#include <logging/Log.h>

class Portal : public Greet::EntityModel
{
  private:
    Greet::Mesh* mesh;
    float delta = 0.0f;
  public:
    Portal(Greet::Vec3<float> position)
      : Greet::EntityModel{nullptr, new Greet::Material{Greet::Shader::FromFile("res/shaders/portal.shader"), Greet::TextureManager::LoadTexture2D("res/textures/portal.meta")}, position, {10,10,10}, {0,0,0}}
    {
      using namespace Greet;
      SetMesh(mesh = new Mesh(MeshFactory::Plane({0, 0, 0}, {2, 2})));
    }

    virtual ~Portal()
    {
      delete mesh;
    }

    void Update(float timeElapsed) override
    {
      delta += timeElapsed;
    }

    void PreRender() const override
    {
      EntityModel::PreRender();
      GetMaterial()->GetShader()->SetUniform1f("time", delta);
    }

};
