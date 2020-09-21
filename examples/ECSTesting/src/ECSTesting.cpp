#include "ECSTesting.h"

#include "CameraController.h"

using namespace Greet;

ECSTesting::ECSTesting()
  : App{"ECSTesting", 1440, 1440 * 9 / 16}
{
  SetFrameCap(60);
}

ECSTesting::~ECSTesting()
{
}

void ECSTesting::Init()
{
  FontManager::Add("noto", FontContainer("res/fonts/noto.ttf"));
  gui = new GUIScene();
  Frame* frame = FrameFactory::GetFrame("res/guis/gui.xml");
  gui->AddFrameQueued(frame);
  GlobalSceneManager::GetSceneManager().Add2DScene(gui, "gui");
  SceneView* sceneView = frame->GetComponentByName<SceneView>("sceneView");
  ASSERT(sceneView, "SceneView did not exist in gui");
  scene.reset(new ECSScene());
  sceneView->GetSceneManager().Add3DScene(scene.get(), "ecs");

  Entity camera = scene->AddEntity();
  camera.AddComponent<Camera3DComponent>(Mat4::Identity(), 90.0f, 0.001f, 1000.0f, true);
  camera.AddComponent<NativeScriptComponent>(Ref<NativeScript>{new CameraController{camera, {2, 2, 2}, {-M_PI / 4, M_PI / 4, 0}}});

  Entity cube = scene->AddEntity();
  cube.AddComponent<Transform3DComponent>(Mat4::Translate(0, 0, 0));
  cube.AddComponent<MeshComponent>(Ref<Mesh>{new Mesh{MeshFactory::Cube()}});
  cube.AddComponent<MaterialComponent>(Ref<Material>{new Material{ShaderFactory::Shader3D()}});

  Entity camera2d = scene->AddEntity();
  camera2d.AddComponent<Camera2DComponent>(Mat3::Identity(), true);

  Entity square = scene->AddEntity();
  square.AddComponent<Transform2DComponent>(Vec2f{20.0f, 20.0f}, Vec2f{20.0f, 20.0f}, M_PI / 4);
}

void ECSTesting::Tick()
{
}

void ECSTesting::Update(float timeElapsed)
{
}

void ECSTesting::Render()
{
}

void ECSTesting::OnEvent(Greet::Event& event)
{
}
