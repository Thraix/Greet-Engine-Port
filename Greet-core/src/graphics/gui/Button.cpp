#include "Button.h"

namespace Greet {

	Button::Button(const Vec2& position, const Vec2 size, const std::string& text)
		: GUI(position,size), m_text(text), m_font(FontManager::Get("anonymous",ceil(size.y * 0.75)))
	{
		m_renderBackground = true;
		m_backgroundColor = ColorUtils::GetMaterialColorAsHSV(120 / 360.0f, 9);
		m_buttonBigger = false;
	}

	void Button::Submit(GUIRenderer* renderer) const
	{
		renderer->SubmitString(m_text, m_size/2.0f + Vec2(-m_font->GetWidthOfText(m_text)/2, m_font->GetSize()*0.25f),m_font, ColorUtils::GetMaterialColorAsHSV(120 / 360.0f, 5));
	}

	void Button::OnMouseEnter()
	{
		if (!m_buttonBigger)
		{
			DriverDispatcher::AddDriver(new RectDriver(m_position.x, m_position.y, m_size.x, m_size.y, -BUTTON_RESIZE_SIZE, -BUTTON_RESIZE_SIZE, BUTTON_RESIZE_SIZE * 2, BUTTON_RESIZE_SIZE * 2, BUTTON_RESIZE_TIME, true, new DriverAdapter()));
			m_buttonBigger = true;
		}
	}

	void Button::OnMouseExit()
	{
		if (m_buttonBigger)
		{
			DriverDispatcher::AddDriver(new RectDriver(m_position.x, m_position.y, m_size.x, m_size.y, BUTTON_RESIZE_SIZE, BUTTON_RESIZE_SIZE, -BUTTON_RESIZE_SIZE * 2, -BUTTON_RESIZE_SIZE * 2, BUTTON_RESIZE_TIME, true, new DriverAdapter()));
			m_buttonBigger = false;
		}
	}

	bool Button::OnMoved(const MouseMovedEvent& event, Vec2 relativeMousePos)
	{
		GUI::OnMoved(event, relativeMousePos);
		return false;
	}

	GUI* Button::OnPressed(const MousePressedEvent& event, Vec2 relativeMousePos)
	{
		return GUI::OnPressed(event, relativeMousePos);
	}
	GUI* Button::OnReleased(const MouseReleasedEvent& event, Vec2 relativeMousePos)
	{
		GUI::OnReleased(event, relativeMousePos);
		return NULL;
	}
}
