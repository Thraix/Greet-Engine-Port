#include <Greet.h>

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

		GLayer::AddFrame(GUIUtils::GetFrame(XML::FromFile("res/guis/test_gui.xml")), "testing");
    Frame* frame = GLayer::GetFrame("testing");
#if 0
    ((ProgressBar*)frame->GetComponentByName("progressBar"))
      ->AttachValueReference(&progressBarValue);
    ((ProgressBar*)frame->GetComponentByName("progressBarVertical"))
      ->AttachValueReference(&progressBarValue);
    ((ProgressBar*)frame->GetComponentByName("progressBarReverse"))
      ->AttachValueReference(&progressBarValue);
    ((ProgressBar*)frame->GetComponentByName("progressBarVerticalReverse"))
      ->AttachValueReference(&progressBarValue);
#endif
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
