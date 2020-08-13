#include <Greet.h>
#include <functional>

#include "TestScene.h"

using namespace Greet;

#define BIND_MEMBER(fn) [this](auto&&... args){ this->fn(std::forward<decltype(args)>(args)...); }

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
        frame->GetComponentByName<ProgressBar>("progressBar")
          ->AttachValueReference(&progressBarValue);
        frame->GetComponentByName<RadioGroup>("Radio")
          ->SetOnRadioChangeCallback(BIND_MEMBER(OnRadioChangeCallback));
        frame->GetComponentByName<Slider>("Slider")
          ->SetOnClickCallback(BIND_MEMBER(OnClickCallback));
        frame->GetComponentByName<Slider>("Slider")
          ->SetOnPressCallback(BIND_MEMBER(OnClickCallback));
        frame->GetComponentByName<Slider>("Slider")
          ->SetOnReleaseCallback(BIND_MEMBER(OnReleaseCallback));
        frame->GetComponentByName<Slider>("Slider")
          ->SetOnValueChangeCallback(BIND_MEMBER(OnValueChangeCallback));
        frame->GetComponentByName<ProgressBar>("progressBar")
          ->AttachValueReference(&progressBarValue);
        frame->GetComponentByName<ProgressBar>("progressBarVertical")
          ->AttachValueReference(&progressBarValue);
        frame->GetComponentByName<ProgressBar>("progressBarReverse")
          ->AttachValueReference(&progressBarValue);
        frame->GetComponentByName<ProgressBar>("progressBarVerticalReverse")
          ->AttachValueReference(&progressBarValue);
        frame->GetComponentByName<ColorPicker>("backgroundChanger")
          ->SetOnColorChangeCallback(BIND_MEMBER(OnColorChangeCallback));
        frame->GetComponentByName<Button>("button")
          ->SetOnClickCallback(BIND_MEMBER(OnButtonPressCallback));
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
