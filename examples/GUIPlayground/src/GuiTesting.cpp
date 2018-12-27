#include <Greet.h>
#include <functional>

using namespace Greet;

class Core : public App, public KeyListener, public MouseListener
{
private:
	Component* content;
  float progressBarValue;
	//Container* container;
public:
  Core()
    : App("GUI Testing", 960, 540)
  {
		SetFrameCap(60);
  }

	~Core()
	{
		GLayer::DestroyInstance();
//		delete container;
	}

	void Init() override
	{
    EventDispatcher::AddKeyListener(10,*this);
    EventDispatcher::AddMouseListener(10,*this);
		FontManager::Add(new FontContainer("/usr/share/fonts/truetype/ubuntu/Ubuntu-C.ttf","roboto"));

    // TODO: This should be done by the engine
		GLayer::CreateInstance(new GUIRenderer(), Shader(Shader::FromFile("res/shaders/gui.shader")));

		GLayer::AddFrame(GUIUtils::GetFrame(XML::FromFile("res/guis/gui.xml")), "testing");
    Frame* frame = GLayer::GetFrame("testing");
#if 1
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
#endif
	}

  void OnRadioChangeCallback(RadioButton* button)
  {
    Log::Info("Radio changed to ", button->GetName());
  }

  void OnValueChangeCallback(Component* component, float oldValue, float newValue)
  {
    Log::Info("Slider changed value from ", oldValue, " to ", newValue);
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
    // TODO: This should be done by the engine
		GLayer::Update(elapsedTime);
    progressBarValue += elapsedTime * 0.5;
    if(progressBarValue >= 1.5)
      progressBarValue -= 1.5;
	}

	void Render() override
	{
    // TODO: This should be done by the engine
		GLayer::Render();
	}

	void OnPressed(const KeyPressedEvent& e) override
	{
	}

	void OnReleased(const KeyReleasedEvent& e)  override
	{
	}

	void OnTyped(const KeyTypedEvent& e) override
	{
	}

	void OnMoved(const MouseMovedEvent& e) override
	{
    //progressBarValue = e.GetX();
	}

	void OnScroll(const MouseScrollEvent& e) override
	{

	}

	void WindowResize(int width, int height) override
	{
	}

	void JoystickState(uint joy, bool connected) override
	{

	}

	void OnClick(GUI* gui)
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
