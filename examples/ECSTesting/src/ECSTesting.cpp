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

  Entity cube = scene->AddEntity("Cube");
  cube.AddComponent<Transform3DComponent>(Mat4::Scale(10, 10, 10));
  cube.AddComponent<MeshComponent>(Ref<Mesh>{new Mesh{MeshFactory::Cube()}});
  cube.AddComponent<MaterialComponent>(Ref<Material>{new Material{ShaderFactory::Shader3D(), TextureManager::LoadTexture2D("res/textures/block.meta")}});

  Entity terrain = scene->AddEntity("Terrain");
  terrain.AddComponent<Transform3DComponent>(Mat4::Scale(1, 20, 1) * Mat4::Translate(0, -0.5f, 0));
  terrain.AddComponent<MeshComponent>(Ref<Mesh>{new Mesh{MeshFactory::Cube()}});
  terrain.AddComponent<MaterialComponent>(Ref<Material>{new Material{Shader::FromFile("res/shaders/terrain.glsl")}});
  terrain.AddComponent<NativeScriptComponent>(Ref<NativeScriptResource>(new NativeScriptResource{"res/scripts/TerrainGeneratorScript.cpp"}));

  Entity water = scene->AddEntity("Water");
  water.AddComponent<Transform3DComponent>(Mat4::Translate(0, -10 + 0.45f * 20.0f, 0));
  water.AddComponent<MeshComponent>(Ref<Mesh>{new Mesh{Greet::MeshFactory::Grid({99, 99}, {}, {0.0f, 0.0f, 0.0f}, {99.0f, 0.0f, 99.0f})}});
  water.AddComponent<MaterialComponent>(Ref<Material>{new Material{Shader::FromFile("res/shaders/water.glsl")}});
  water.AddComponent<NativeScriptComponent>(Ref<NativeScriptResource>(new NativeScriptResource{"res/scripts/WaterScript.cpp"}));

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
