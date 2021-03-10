#include "EditorScene.h"


using namespace Greet;

EditorScene::EditorScene()
{
  lineShader = Shader::FromFile("res/shaders/simple.shader");
  lineMesh = NewRef<Mesh>(
    Pointer<Vec3f>{
      {0.5f, 0.5f, 0.5f},
      {-0.5f, 0.5f, 0.5f},
      {0.5f, -0.5f, 0.5f},
      {-0.5f, -0.5f, 0.5f},
      {0.5f, 0.5f, -0.5f},
      {-0.5f, 0.5f, -0.5f},
      {0.5f, -0.5f, -0.5f},
      {-0.5f, -0.5f, -0.5f},
    },
    Pointer<uint>{0, 1, 1, 3, 3, 2, 2, 0, 4, 5, 5, 7, 7, 6, 6, 4, 0, 4, 1, 5, 2, 6, 3, 7}
  );
  lineMesh->SetDrawMode(DrawType::LINES);
}

void EditorScene::Render3D() const
{
  ECSScene::Render3D();
  if(selectedEntity)
  {
    Entity camera{manager.get()};
    bool foundPrimary = false;
    manager->Each<Camera3DComponent>([&](EntityID id, Camera3DComponent& cam)
    {
      if(cam.active)
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
    glLineWidth(3);
    lineShader->Enable();
    camera.GetComponent<Camera3DComponent>().SetShaderUniforms(lineShader);
    lineShader->SetUniformColor4("uMaterialColor", Color{1, 0, 1});
    lineShader->SetUniformMat4("uTransformationMatrix", selectedEntity.GetComponent<Transform3DComponent>().transform );
    lineMesh->Bind();
    lineMesh->Render();
    lineMesh->Unbind();
    lineShader->Disable();
    glLineWidth(1);
  }
}

void EditorScene::OnEvent(Event& event)
{
  ECSScene::OnEvent(event);
  if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
  {
    MousePressEvent e = static_cast<MousePressEvent&>(event);
    Entity camera{manager.get()};
    bool foundPrimary = false;
    manager->Each<Camera3DComponent>([&](EntityID id, Camera3DComponent& cam)
    {
      if(cam.active)
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

    int i = 0;
    float distanceSq = 10000; // Limit
    Entity collision{manager.get()};
    Camera3DComponent& component = camera.GetComponent<Camera3DComponent>();
    manager->Each<Transform3DComponent, MeshComponent, MaterialComponent>([&](EntityID id, Transform3DComponent& transform, MeshComponent& mesh, MaterialComponent& material)
    {
      Line line = component.GetScreenToWorldCoordinate(Mat4::Inverse(transform.transform), e.GetPosition());
      Vec3f pointXP = Plane{Vec3f{1, 0, 0}, Vec3f{0.5f, 0, 0}}.LineIntersection(line);
      Vec3f pointYP = Plane{Vec3f{0, 1, 0}, Vec3f{0, 0.5f, 0}}.LineIntersection(line);
      Vec3f pointZP = Plane{Vec3f{0, 0, 1}, Vec3f{0, 0, 0.5f}}.LineIntersection(line);
      Vec3f pointXN = Plane{Vec3f{-1, 0, 0}, Vec3f{-0.5f, 0, 0}}.LineIntersection(line);
      Vec3f pointYN = Plane{Vec3f{0, -1, 0}, Vec3f{0, -0.5f, 0}}.LineIntersection(line);
      Vec3f pointZN = Plane{Vec3f{0, 0, -1}, Vec3f{0, 0, -0.5f}}.LineIntersection(line);

      if(pointXP.y > -0.5f && pointXP.y < 0.5f && pointXP.z > -0.5f && pointXP.z < 0.5f && (pointXP - component.GetPosition()).LengthSQ() < distanceSq)
      {
        collision.SetID(id);
        distanceSq = (pointXP - component.GetPosition()).LengthSQ();
      }
      if(pointYP.x > -0.5f && pointYP.x < 0.5f && pointYP.z > -0.5f && pointYP.z < 0.5f && (pointYP - component.GetPosition()).LengthSQ() < distanceSq)
      {
        collision.SetID(id);
        distanceSq = (pointYP - component.GetPosition()).LengthSQ();
      }
      if(pointZP.x > -0.5f && pointZP.x < 0.5f && pointZP.y > -0.5f && pointZP.y < 0.5f && (pointZP - component.GetPosition()).LengthSQ() < distanceSq)
      {
        collision.SetID(id);
        distanceSq = (pointZP - component.GetPosition()).LengthSQ();
      }
      if(pointXN.y > -0.5f && pointXN.y < 0.5f && pointXN.z > -0.5f && pointXN.z < 0.5f && (pointXN - component.GetPosition()).LengthSQ() < distanceSq)
      {
        collision.SetID(id);
        distanceSq = (pointXN - component.GetPosition()).LengthSQ();
      }
      if(pointYN.x > -0.5f && pointYN.x < 0.5f && pointYN.z > -0.5f && pointYN.z < 0.5f && (pointYN - component.GetPosition()).LengthSQ() < distanceSq)
      {
        collision.SetID(id);
        distanceSq = (pointYN - component.GetPosition()).LengthSQ();
      }
      if(pointZN.x > -0.5f && pointZN.x < 0.5f && pointZN.y > -0.5f && pointZN.y < 0.5f && (pointZN - component.GetPosition()).LengthSQ() < distanceSq)
      {
        collision.SetID(id);
        distanceSq = (pointZN - component.GetPosition()).LengthSQ();
      }
    });
    selectedEntity = collision;
    if(selectedEntity)
      Log::Info("selected entity");
  }

}
