#include "ECSScene.h"

#include <ecs/ECSComponents.h>
#include <graphics/RenderCommand.h>
#include <graphics/textures/TextureManager.h>

using namespace Greet;

ECSScene::ECSScene()
  : manager{new ECSManager()}, skybox{new Skybox{TextureManager::LoadCubeMap("res/textures/skybox.meta")}}
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
  Entity camera{manager};
  bool foundPrimary = false;
  manager->Each<Camera3DComponent>([&](EntityID id, Camera3DComponent& cam)
  {
    if(cam.primary)
    {
      if(foundPrimary)
        Log::Warning("More than one primary camera in scene");
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
  }
  manager->Each<NativeScriptComponent>([&](EntityID id, NativeScriptComponent& script)
  {
    if(script.created)
      script.OnEvent(event);
  });
}
