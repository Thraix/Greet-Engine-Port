#include "ECSTesting.h"

using namespace Greet;

struct PrintComponent
{
};

class ECSSceneExt : public ECSScene
{
  public:
    ECSSceneExt(const std::string& scenePath)
      : ECSScene{scenePath}
    {}

    virtual ~ECSSceneExt()
    {}

    void LoadExtComponents(Entity& entity, const MetaFile& metaClass) override
    {
    }

    void UpdateAfter(float timeElapsed) override
    {
    }
};

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
  Ref<GUIScene> gui = NewRef<GUIScene>();
  Frame* frame = FrameFactory::GetFrame("res/guis/gui.xml");
  gui->AddFrameQueued(frame);
  GlobalSceneManager::GetSceneManager().Add2DScene(gui, "gui");
  sceneView = frame->GetComponentByName<SceneView>("sceneView");
  ASSERT(sceneView, "SceneView did not exist in gui");
  Ref<ECSSceneExt> scene = NewRef<ECSSceneExt>("res/scenes/scene.meta");
  sceneView->GetSceneManager().Add3DScene(scene, "ecs");
  gui->RequestFocusQueued(sceneView);
}

void ECSTesting::Destruct()
{
  sceneView->GetSceneManager().Remove3DScene("ecs");
  GlobalSceneManager::GetSceneManager().Remove2DScene("gui");
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
