#include "ECSScene.h"

#include <ecs/ECSComponents.h>
#include <graphics/RenderCommand.h>
#include <graphics/textures/TextureManager.h>

namespace Greet
{

  ECSScene::ECSScene()
    : manager{new ECSManager()}, renderer2d{new BatchRenderer(ShaderFactory::Shader2D())}
  {
  }

  ECSScene::~ECSScene()
  {
    TextureManager::CleanupUnused();
  }

  Greet::Entity ECSScene::AddEntity(const std::string& tag)
  {
    Entity e = Entity::Create(manager);
    e.AddComponent<TagComponent>(tag);
    return e;
  }

  void ECSScene::RemoveEntity(const Entity& entity)
  {
    manager->DestroyEntity(entity.GetID());
  }

  void ECSScene::RemoveEntity(EntityID entity)
  {
    manager->DestroyEntity(entity);
  }

  void ECSScene::Render() const
  {
    Render3D();
    Render2D();
  }

  void ECSScene::Render2D() const
  {
    Entity camera{manager};
    manager->Each<Camera2DComponent>([&](EntityID id, Camera2DComponent& cam)
    {
      if(cam.primary)
      {
        if(camera)
          Log::Warning("More than one primary 2D camera in scene");
        camera.SetID(id);
      }
    });

    // No camera found, so cannot render anything
    if(!camera)
      return;

    Entity environment{manager};
    manager->Each<Environment2DComponent>([&](EntityID id, Environment2DComponent& env)
        {
        if(environment)
        Log::Warning("More than one environment in 2D scene");
        environment.SetID(id);
        });

    Camera2DComponent& cam = camera.GetComponent<Camera2DComponent>();
    if(environment)
      renderer2d->SetShader(environment.GetComponent<Environment2DComponent>().shader);

    renderer2d->Begin();
    cam.SetShaderUniforms(renderer2d->GetShader());
    manager->Each<Transform2DComponent>([&](EntityID id, Transform2DComponent& transform)
    {
      Entity e{manager, id};
      if(e.HasComponent<SpriteComponent>())
      {
        SpriteComponent& sprite = e.GetComponent<SpriteComponent>();
        renderer2d->DrawRect(transform.transform, sprite.texture, sprite.texPos, sprite.texSize);
      }
      else
        renderer2d->DrawRect(transform.transform);
    });
    renderer2d->End();
    renderer2d->Flush();
  }

  void ECSScene::Render3D() const
  {
    Entity camera{manager};
    bool foundPrimary = false;
    manager->Each<Camera3DComponent>([&](EntityID id, Camera3DComponent& cam)
    {
      if(cam.primary)
      {
        if(foundPrimary)
          Log::Warning("More than one primary 3D camera in scene");
        foundPrimary = true;
        camera.SetID(id);
      }
    });

    if(!foundPrimary)
    {
      Log::Warning("No camera in scene");
      return;
    }

    Entity environment{manager};
    manager->Each<Environment3DComponent>([&](EntityID id, Environment3DComponent& env)
    {
      if(environment)
        Log::Warning("More than one environment in 3D scene");
      environment.SetID(id);
    });
    Camera3DComponent& cam = camera.GetComponent<Camera3DComponent>();

    Environment3DComponent* env = nullptr;
    if(environment)
    {
      env = &environment.GetComponent<Environment3DComponent>();
      env->Skybox(cam);
    }

    manager->Each<Transform3DComponent, MeshComponent, MaterialComponent>([&](EntityID id, Transform3DComponent& transform, MeshComponent& mesh, MaterialComponent& material)
    {
      material.material->Bind();
      cam.SetShaderUniforms(material.material->GetShader());
      material.material->GetShader()->SetUniformMat4("uTransformationMatrix", transform.transform);
      material.material->GetShader()->SetUniform3f("uLightPosition", Vec3f(30.0f, 50.0f, 40.0f));
      material.material->GetShader()->SetUniform3f("uLightColor", Vec3f(0.7, 0.7, 0.7));
      if(env)
        env->SetShaderUniforms(material.material->GetShader());
      mesh.mesh->Bind();
      mesh.mesh->Render();
      mesh.mesh->Unbind();
      material.material->Unbind();
    });
  }

  void ECSScene::Update(float timeElapsed)
  {
    manager->Each<NativeScriptComponent>([&](EntityID id, NativeScriptComponent& script)
    {
      if(!script.script->HasBoundEntity())
        script.BindEntity({manager, id});
      if(!script.created)
      {
        script.Create();
      }
      script.Update(timeElapsed);
    });
  }

  void ECSScene::OnEvent(Greet::Event& event)
  {
    if(EVENT_IS_TYPE(event, EventType::VIEWPORT_RESIZE))
    {
      ViewportResizeEvent& e = static_cast<ViewportResizeEvent&>(event);
      manager->Each<Camera3DComponent>([&](EntityID id, Camera3DComponent& cam)
      {
        cam.ViewportResize(e);
      });
  manager->Each<Camera2DComponent>([&](EntityID id, Camera2DComponent& cam)
      {
        cam.ViewportResize(e);
      });
    }
    manager->Each<NativeScriptComponent>([&](EntityID id, NativeScriptComponent& script)
    {
      if(script.created)
        script.OnEvent(event);
    });
  }
}
