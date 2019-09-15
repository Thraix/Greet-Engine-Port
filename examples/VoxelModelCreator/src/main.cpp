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
        FontManager::Add(new FontContainer("res/fonts/NotoSansUI-Regular.ttf","noto"));
        SetFrameCap(144);

        guiScene = new GUIScene(new GUIRenderer(), Shader::FromFile("res/shaders/gui.shader"));
        guiScene->AddFrame(FrameFactory::GetFrame("res/guis/editor.xml"));

        Loaders::LoadTextures("res/loaders/textures.json");
        // Load Textures and Fonts.
        FontManager::Add(new FontContainer("Anonymous Pro.ttf", "anonymous"));


        grid = new Grid();
        GlobalSceneManager::GetSceneManager().Add2DScene(guiScene, "GUIScene");
        Frame* frame = guiScene->GetFrame("Frame");
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
            [=](Component*, const Vec3<float>& prev, const Vec3<float>& cur) -> void
            {
            grid->SetColor(ColorUtils::Vec3ToColorHex(cur));
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
