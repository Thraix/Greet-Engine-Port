#pragma once

#include <graphics/gui/Frame.h>
#include <graphics/shaders/Shader.h>
#include <event/MouseListener.h>
#include <event/KeyListener.h>
#include <listeners/WindowResize.h>
#include <string>
#include <map>

namespace Greet {
	class GLayer : public MouseListener, public KeyListener, public WindowResizeListener
	{
	private:
		static std::map<std::string, Frame*> frames;
		static GLayer* instance;
		Component* m_focused;
		Component* m_hovered;
		GUIRenderer* m_renderer;
		Shader m_shader;

	private:
		GLayer(GUIRenderer* renderer, const Shader& shader);
	public:

		bool OnPressed(const MousePressedEvent& event) override;
		void OnReleased(const MouseReleasedEvent& event) override;
		void OnMoved(const MouseMovedEvent& event) override;
		void OnPressed(const KeyPressedEvent& event) override;
		void OnReleased(const KeyReleasedEvent& event) override;
    void OnTyped(const KeyTypedEvent& event) override;
		void WindowResize(int width, int height) override;


		static void CreateInstance(GUIRenderer* renderer, const Shader& shader);
		static GLayer* GetInstance();
		static void DestroyInstance();

		static void Render();
		static void Update(float timeElapsed);

		static void AddFrame(Frame* frame, const std::string& name);
		static Frame* RemoveFrame(const std::string& name);
		static Frame* RemoveFrame(Frame* frame);
		static Frame* GetFrame(const std::string& name);

    static float GetWidth();
    static float GetHeight();

	};
}
