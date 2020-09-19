#include "ECSTesting.h"

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
  camera.AddComponent<Camera3DComponent>(Mat4::ViewMatrix({0, 0, 0}, {}), 90, 0.001, 1000.0f, true);
  Entity cube = scene->AddEntity();
  cube.AddComponent<Transform3DComponent>(Mat4::Translate(0, 0, -2));
  cube.AddComponent<MeshComponent>(Ref<Mesh>{new Mesh{MeshFactory::Cube()}});
  cube.AddComponent<MaterialComponent>(Ref<Material>{new Material{ShaderFactory::Shader3D()}});
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
