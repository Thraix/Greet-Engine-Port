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
  gui.reset(new GUIScene());
  Frame* frame = FrameFactory::GetFrame("res/guis/gui.xml");
  gui->AddFrameQueued(frame);
  GlobalSceneManager::GetSceneManager().Add2DScene(gui, "gui");
  SceneView* sceneView = frame->GetComponentByName<SceneView>("sceneView");
  ASSERT(sceneView, "SceneView did not exist in gui");
  scene.reset(new ECSScene("res/scenes/scene.meta"));
  sceneView->GetSceneManager().Add3DScene(scene, "ecs");
  gui->RequestFocusQueued(sceneView);

  Entity camera = scene->AddEntity("CameraEnvironmnet3D");
  camera.AddComponent<Camera3DComponent>(Mat4::Identity(), 90.0f, 0.01f, 100.0f, true);
  Environment3DComponent& env3d = camera.AddComponent<Environment3DComponent>(TextureManager::LoadCubeMap("res/textures/skybox.meta"));
  env3d.fogNearDistance = 40;
  env3d.fogFarDistance = 80;
  camera.AddComponent<NativeScriptComponent>(Ref<NativeScriptResource>(new NativeScriptResource{"res/scripts/CameraControllerScript.cpp"}));

  Entity env2d = scene->AddEntity("CameraEnvironmnet2D");
  env2d.AddComponent<Camera2DComponent>(Mat3::Identity(), true);
  env2d.AddComponent<Environment2DComponent>(Shader::FromFile("res/shaders/shader2d.glsl"));

  Entity square = scene->AddEntity("Square");
  square.AddComponent<Transform2DComponent>(Vec2f{50.0f, 50.0f}, Vec2f{100.0f, 100.0f}, M_PI / 4);
  square.AddComponent<SpriteComponent>(TextureManager::LoadTexture2D("res/textures/sprite.meta"), Vec2f{0.0f, 0.0f}, Vec2f{2.0f, 2.0f});
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
