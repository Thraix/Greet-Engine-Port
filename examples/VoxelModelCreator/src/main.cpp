#include <Greet.h>

#include <random>

#include <functional>
#include <utility>
#include "Grid.h"

using namespace Greet;
namespace vmc
{
  class Core : public App
  {
    private:
      Grid* grid;
      Renderable2D* cursor;
      GUIScene* guiScene;

    public:

      Core()
        : App("Voxel Model Creator", 1280, 720)
      {

      }

      ~Core()
      {
        delete grid;
        delete guiScene;
      }

      void Init() override
      {
        using namespace std::placeholders;
        FontManager::Add("noto", FontContainer("res/fonts/NotoSansUI-Regular.ttf"));
        SetFrameCap(144);

        guiScene = new GUIScene();
        Frame* frame = FrameFactory::GetFrame("res/guis/editor.xml");
        guiScene->AddFrameQueued(frame);

        FontManager::Add("anonymous", FontContainer("Anonymous Pro.ttf"));

        grid = new Grid();
        GlobalSceneManager::GetSceneManager().Add2DScene(guiScene, "GUIScene");
        if(!frame)
        {
          Log::Error("No frame called \"Frame\" in GUI");
          GlobalSceneManager::GetSceneManager().Add3DScene(grid, "gridLayer");
          return;
        }
        SceneView* sceneView= frame->GetComponentByName<SceneView>("viewport");
        if(!sceneView)
        {
          Log::Error("No viewport called \"viewport\" in GUI");
          GlobalSceneManager::GetSceneManager().Add3DScene(grid, "gridLayer");
          return;
        }
        sceneView->GetSceneManager().Add3DScene(grid, "gridLayer");
        ColorPicker* colorPicker = frame->GetComponentByName<ColorPicker>("cubeColor");
        if(!colorPicker)
        {
          Log::Error("No ColorPicker called \"cubeColor\" in GUI");
          return;
        }
        colorPicker->SetOnColorChangeCallback(
            [=](Component*, const Color& prev, const Color& cur) -> void
            {
            grid->SetColor(cur.AsUInt());
            });
      }

      void Tick() override
      {
        std::string s = StringUtils::to_string(GetFPS()) + " fps | " + StringUtils::to_string(GetUPS()) + " ups";
        Window::SetTitle("Best Game Ever | " + s);
      }

      float d = 0;
      void Update(float elapsedTime) override
      {
      }

      bool screenshot = false;
      void Render() override
      {
      }

      void OnEvent(Event& event) override
      {
        if(EVENT_IS_TYPE(event, EventType::KEY_PRESS))
        {
          KeyPressEvent& kEvent = (KeyPressEvent&)event;
          if(kEvent.GetButton() == GLFW_KEY_F10)
            Utils::Screenshot(Window::GetWidth(), Window::GetHeight());
        }
      }
  };
}

int main()
{
  vmc::Core game;
  game.Start();
}
