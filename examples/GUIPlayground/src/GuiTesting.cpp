#include <Greet.h>
#include <functional>

#include "TestScene.h"

using namespace Greet;

class Core : public App
{
  private:
    GUIScene* guiScene;
    Component* content;
    TestScene* scene;
    float progressBarValue;
  public:
    Core()
      : App("GUI Testing", 960, 540)
    {
      SetFrameCap(60);
    }

    ~Core()
    {
      /* delete guiScene; */
    }

    void Init() override
    {
      /* GlobalSceneManager::GetSceneManager().Add2DScene(new TestScene(), "testscene"); */
      /* return; */
      progressBarValue = 0;
      FontManager::Add("noto", FontContainer("res/fonts/NotoSansUI-Regular.ttf"));

      guiScene = new GUIScene(new GUIRenderer());

      guiScene->AddFrame(FrameFactory::GetFrame("res/guis/gui.xml"));

#if 1
      Frame* frame = guiScene->GetFrame("Main");
      if(frame != nullptr)
      {
        using namespace std::placeholders;
        // Function callbacks
        // Somewhat ugly, might look into making function pointers easier, since the structure
        // is always the same.
        frame->GetComponentByName<ProgressBar>("progressBar")
          ->AttachValueReference(&progressBarValue);
        frame->GetComponentByName<RadioGroup>("Radio")
          ->SetOnRadioChangeCallback(std::bind(&Core::OnRadioChangeCallback, std::ref(*this), _1));
        frame->GetComponentByName<Slider>("Slider")
          ->SetOnClickCallback(std::bind(&Core::OnClickCallback, std::ref(*this), _1));
        frame->GetComponentByName<Slider>("Slider")
          ->SetOnPressCallback(std::bind(&Core::OnPressCallback, std::ref(*this), _1));
        frame->GetComponentByName<Slider>("Slider")
          ->SetOnReleaseCallback(std::bind(&Core::OnReleaseCallback, std::ref(*this), _1));
        frame->GetComponentByName<Slider>("Slider")
          ->SetOnValueChangeCallback(std::bind(&Core::OnValueChangeCallback, std::ref(*this), _1, _2, _3));
        frame->GetComponentByName<ProgressBar>("progressBar")
          ->AttachValueReference(&progressBarValue);
        frame->GetComponentByName<ProgressBar>("progressBarVertical")
          ->AttachValueReference(&progressBarValue);
        frame->GetComponentByName<ProgressBar>("progressBarReverse")
          ->AttachValueReference(&progressBarValue);
        frame->GetComponentByName<ProgressBar>("progressBarVerticalReverse")
          ->AttachValueReference(&progressBarValue);
        frame->GetComponentByName<ColorPicker>("backgroundChanger")
          ->SetOnColorChangeCallback(std::bind(&Core::OnColorChangeCallback, std::ref(*this), _1, _2, _3));
        frame->GetComponentByName<Button>("button")
          ->SetOnClickCallback(std::bind(&Core::OnButtonPressCallback, std::ref(*this), _1));
        Docker* docker = frame->GetComponentByName<Docker>("docker");
        if(docker)
        {
          DockerTab* tab = docker->GetTab("Settings");
          if(tab)
          {
            tab->SetOnTabShowCallback(
                [](Docker* docker, DockerTab* tab)
                {
                  Log::Info("Show");
                });
            tab->SetOnTabHideCallback(
                [](Docker* docker, DockerTab* tab)
                {
                  Log::Info("Hide");
                });
          }
        }
      }
#endif
      GlobalSceneManager::GetSceneManager().Add2DScene(guiScene, "GUIScene");
    }

    void OnRadioChangeCallback(RadioButton* button)
    {
      Log::Info("Radio changed to ", button->GetName());
    }

    void OnButtonPressCallback(Component* button)
    {
      Log::Info("Button pressed: ", button->GetName());
    }

    void OnValueChangeCallback(Component* component, float oldValue, float newValue)
    {
      Log::Info("Slider changed value from ", oldValue, " to ", newValue);
    }

    void OnColorChangeCallback(Component* component, const Vec3<float>& oldValue, const Vec3<float>& current)
    {
      Vec4 color = {current.r, current.g, current.b, 1.0};
      RenderCommand::SetClearColor(Vec4(current.r,current.g,current.b,1));
      Component* editorContainer = guiScene->GetFrame("Main")->GetComponentByName<Component>("EditorContainer");
      editorContainer->LoadStyle("normal", Styling{.colors={{"backgroundColor", color}}});
    }

    void OnClickCallback(Component* component)
    {
      Log::Info("On click");
    }

    void OnPressCallback(Component* component)
    {
      Log::Info("On Press");
    }

    void OnReleaseCallback(Component* component)
    {
      Log::Info("On Release");
    }

    void Tick() override
    {
      std::string s = StringUtils::to_string(GetFPS()) + " fps | " + StringUtils::to_string(GetUPS()) + " ups";
      Window::SetTitle("GUI Testing | " + s);
    }

    void Update(float elapsedTime) override
    {
      progressBarValue += elapsedTime * 0.5;
      if(progressBarValue >= 1.5)
        progressBarValue -= 1.5;
    }

    void Render() override
    {
    }
};

int main()
{
  {
    //XMLObject obj = XML::FromFile("projtest.xml");
    Core game;
    game.Start();


  }
}
