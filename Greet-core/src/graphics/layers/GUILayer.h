#pragma once

#include <graphics/layers/Layer.h>
#include <event/MouseListener.h>
#include <event/KeyListener.h>
#include <graphics/Window.h>
#include <utils/Utils.h>
#include <graphics/gui/GUI.h>

namespace Greet {

	class GUILayer : public KeyListener, public MouseListener, public WindowResizeListener
	{

	private:
		GUIRenderer* m_renderer;
		std::vector<GUI*> m_guis;
		Shader* m_shader;
		Mat3 m_projectionMatrix;

		GUI* m_focusedGUI;
	public:
		GUILayer(GUIRenderer* renderer, Shader* shader);
		virtual ~GUILayer();

		void Add(GUI* renderable);
		bool Update(float timeElapsed);
		void Render() const;

		void SetProjectionMatrix(Mat3 projectionMatrix);

		// I/O
		bool OnPressed(const KeyPressedEvent& e) override;
		bool OnReleased(const KeyReleasedEvent& e) override;
		bool OnTyped(const KeyTypedEvent& e) override;
		bool OnPressed(const MousePressedEvent& e) override;
		bool OnReleased(const MouseReleasedEvent& e) override;
		bool OnMoved(const MouseMovedEvent& e) override;
		bool OnScroll(const MouseScrollEvent& e) override;
		void WindowResize(int width, int height) override;
	};

}