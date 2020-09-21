#include "ECSScene.h"

#include <ecs/ECSComponents.h>
#include <graphics/RenderCommand.h>
#include <graphics/textures/TextureManager.h>

using namespace Greet;

ECSScene::ECSScene()
  : manager{new ECSManager()}, skybox{new Skybox{TextureManager::LoadCubeMap("res/textures/skybox.meta")}}, renderer2d{new BatchRenderer(ShaderFactory::Shader2D())}
{
}

Greet::Entity ECSScene::AddEntity()
{
  return Entity::Create(manager);
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
  manager->Each<Environment2D>([&](EntityID id, Environment2D& env)
  {
    if(environment)
      Log::Warning("More than one environment in 2D scene");
    environment.SetID(id);
  });

  Camera2DComponent& cam = camera.GetComponent<Camera2DComponent>();
  if(environment)
    renderer2d->SetShader(environment.GetComponent<Environment2D>().shader);

  renderer2d->Begin();
  cam.SetShaderUniforms(renderer2d->GetShader());
  manager->Each<Transform2DComponent>([&](EntityID id, Transform2DComponent& transform)
  {
    renderer2d->DrawRect(transform.transform, 0xffffffff);
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

  Camera3DComponent& cam = camera.GetComponent<Camera3DComponent>();
  skybox->Render(cam);
  manager->Each<Transform3DComponent, MeshComponent, MaterialComponent>([&](EntityID id, Transform3DComponent& transform, MeshComponent& mesh, MaterialComponent& material)
  {
    material.material->Bind();
    cam.SetShaderUniforms(material.material->GetShader());
    material.material->GetShader()->SetUniformMat4("uTransformationMatrix", transform.transform);
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
    if(!script.created)
      script.Create();
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
