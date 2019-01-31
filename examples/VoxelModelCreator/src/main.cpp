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
		Layer* uilayer;
		Renderable2D* cursor;
    GUIScene* guiScene;

	public:

    Core()
      : App("Voxel Model Creator", 1280, 720)
    {
    
    }

		~Core()
		{
      delete uilayer;
      delete grid;
      delete guiScene;
		}

		void Init() override
		{
      using namespace std::placeholders;
      FontManager::Add(new FontContainer("/usr/share/fonts/truetype/ubuntu/Ubuntu-C.ttf","roboto"));
      EventDispatcher::AddGlobalEventReceiver("Application", std::bind(&Core::OnEvent, std::ref(*this), _1));
			SetFrameCap(144);

      guiScene = new GUIScene(new GUIRenderer(), Shader::FromFile("res/shaders/gui.shader"));
      guiScene->AddFrame(FrameFactory::GetFrame("res/guis/editor.xml"));

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
			//RenderEngine::Add2DScene(uilayer, "uilayer");
			RenderEngine::Add2DScene(guiScene, "GUIScene");
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
		}

		bool screenshot = false;
		void Render() override
		{
		}

    void OnEvent(Event& event)
    {
      if(EVENT_IS_TYPE(event, EventType::WINDOW_RESIZE))
      {
        uilayer->SetProjectionMatrix(Mat3::Orthographic(0,Window::GetWidth(), 0, Window::GetHeight()));
      }
      if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
      {
        MouseMoveEvent& mEvent = (MouseMoveEvent&)event;
        Vec2 pos = ~ uilayer->GetProjectionMatrix() * mEvent.GetPosition();
        cursor->SetPosition(pos);
      }
      else if(EVENT_IS_TYPE(event, EventType::KEY_PRESS))
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
