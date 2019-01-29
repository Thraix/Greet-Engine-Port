#include <Greet.h>

#include <random>

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
      FontManager::Add(new FontContainer("/usr/share/fonts/truetype/ubuntu/Ubuntu-C.ttf","roboto"));
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
/*
		void OnPressed(const KeyPressedEvent& e) override
		{
			if (e.GetButton() == GLFW_KEY_F10)
			{
				Utils::Screenshot(Window::GetWidth(), Window::GetHeight());
			}
		}

		void OnMoved(const MouseMovedEvent& e) override
		{
			cursor->SetPosition(Vec2(e.GetX(), e.GetY()));
		}
    */

		bool screenshot = false;
		void Render() override
		{
		}
	};
}

int main()
{
	vmc::Core game;
	game.Start();
}
