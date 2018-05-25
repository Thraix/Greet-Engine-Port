#pragma once

#include <graphics/gui/GUI.h>
#include <graphics/fonts/FontManager.h>
#include <drivers/RectDriver.h>
#include <graphics/layers/GUILayer.h>
#include <drivers/DriverDispatcher.h>

#define BUTTON_RESIZE_TIME 0.08f
#define BUTTON_RESIZE_SIZE 3.0f

namespace Greet {
		
	class Button : public GUI
	{

	private:
		Font* m_font;
		std::string m_text;
		bool m_buttonBigger;

	public:
		Button(const Vec2& position, const Vec2 size, const std::string& text);
		void Submit(GUIRenderer* renderer) const override;

		void OnMouseEnter();
		void OnMouseExit();
		bool OnMoved(const MouseMovedEvent& event, Vec2 relativeMousePos) override;
		GUI* OnPressed(const MousePressedEvent& event, Vec2 relativeMousePos) override;
		GUI* OnReleased(const MouseReleasedEvent& event, Vec2 relativeMousePos) override;
	};
}