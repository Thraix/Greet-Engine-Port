#include <Greet.h>

#include <random>

#include "Grid.h"

using namespace Greet;
namespace vmc 
{
	class Core : public App, public KeyListener, public MouseListener
	{
	private:
		Grid* grid;
		Layer* uilayer;
		Renderable2D* cursor;

	public:

    Core()
      : App("Voxel Model Creator", 1280, 720)
    {
    
    }

		~Core()
		{
      delete uilayer;
      delete grid;
      GLayer::DestroyInstance();
		}

		void Init() override
		{
			EventDispatcher::AddKeyListener(DISPATCHER_GUI + 1, *this);
			EventDispatcher::AddMouseListener(DISPATCHER_GUI + 1, *this);
      FontManager::Add(new FontContainer("/usr/share/fonts/truetype/ubuntu/Ubuntu-C.ttf","roboto"));
			SetFrameCap(144);

      GLayer::CreateInstance(new GUIRenderer(), Shader::FromFile("res/shaders/gui.shader"));
      GLayer::AddFrame(FrameFactory::GetFrame("res/guis/editor.xml"));

      InputControl* mousePos = new InputControl("mousePos",2);
      InputControl* mouseMiddle = new InputControl("mouseMiddle",1);
      InputControl* mouseScroll = new InputControl("mouseScroll",2);
      InputControl* mouseLeft = new InputControl("mouseLeft",1);
      InputControl* mouseRight = new InputControl("mouseRight",1);

      InputController::AddMouseButton(0, mouseLeft, 1,true,0);
      InputController::AddMouseButton(1, mouseRight, 1,true,0);
      InputController::AddMouseButton(2, mouseMiddle, 1,false,0);
      InputController::AddMouseWheel(0, mouseScroll, 1,true,0);

      InputController::AddMouseMotion(0, mousePos, 1,true,0);
      InputController::AddMouseMotion(1, mousePos, 1,true,1);

			// Load Textures and Fonts.
			TextureManager::Add("cursor", Texture2D("res/textures/cursor.png"));
			TextureManager::Add("mask", Texture2D("res/textures/mask.png",TextureParams(
        TextureFilter::LINEAR, 
        TextureWrap::CLAMP_TO_EDGE, 
        TextureInternalFormat::RGB,
        TextureFormat::INVERTED)));
			TextureManager::Add("guimask", Texture2D("res/textures/guimask.png"));
			TextureManager::Add("skybox", CubeMap("res/textures/skybox.png"));
			FontManager::Add(new FontContainer("Anonymous Pro.ttf", "anonymous"));

			cursor = new Renderable2D(Vec2(0, 0), Vec2(32, 32), 0xffffffff, new Sprite(TextureManager::Get2D("cursor")), new Sprite(TextureManager::Get2D("mask")));
			uilayer = new Layer(new BatchRenderer(), ShaderFactory::DefaultShader(), Mat3::Orthographic(0.0f, (float)Window::GetWidth(), 0.0f, (float)Window::GetHeight()));
			uilayer->Add(cursor);

			grid = new Grid();
			RenderEngine::Add2DScene(uilayer, "uilayer");
			RenderEngine::Add3DScene(grid, "gridLayer");
		}

		void Tick() override
		{
			std::string s = StringUtils::to_string(GetFPS()) + " fps | " + StringUtils::to_string(GetUPS()) + " ups";
			Window::SetTitle("Best Game Ever | " + s);
		}

		float d = 0;
		void Update(float elapsedTime) override
		{
      GLayer::Update(elapsedTime);
		}

		void OnPressed(const KeyPressedEvent& e) override
		{
			if (e.GetButton() == GLFW_KEY_F10)
			{
				Utils::Screenshot(Window::GetWidth(), Window::GetHeight());
			}
		}

		void OnReleased(const KeyReleasedEvent& e)  override
		{
		}

		void OnTyped(const KeyTypedEvent& e) override
		{
		}

		void OnMoved(const MouseMovedEvent& e) override
		{
			cursor->SetPosition(Vec2(e.GetX(), e.GetY()));
		}

		void OnScroll(const MouseScrollEvent& e) override
		{
			d -= e.GetScroll();
		}

		bool screenshot = false;
		void Render() override
		{
      GLayer::Render();
		}

		void WindowResize(int width, int height) override
		{
			uilayer->SetProjectionMatrix(Mat3::Orthographic(0, width, 0, height));
		}

		void JoystickState(uint joy, bool connected) override
		{

		}
	};
}

int main()
{
	vmc::Core game;
	game.Start();
}
