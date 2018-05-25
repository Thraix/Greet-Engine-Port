#include "Frame.h"
#include <graphics/Window.h>
namespace Greet {

	Frame::Frame(const Vec2& position, const Vec2& size, const std::string& title)
	: GUI(position,size, LTRB(10,40,10,10)), m_title(title),m_titleFont(FontManager::Get("anonymous", 20)),
		m_titleColor(ColorUtils::GetMaterialColorAsHSV(120 / 360.0f, 3)),
		m_titleBackgroundColor(ColorUtils::GetMaterialColorAsHSV(120 / 360.0f, 9))
	{
		m_size += Vec2(m_margin.left+m_margin.right, m_margin.top+m_margin.bottom);
	}

	void Frame::Submit(GUIRenderer* renderer) const
	{
		renderer->SubmitRect(Vec2(0, 0), Vec2(m_size.x, m_margin.top-10),m_titleBackgroundColor);
		renderer->SubmitString(m_title, Vec2(m_margin.left, 22),m_titleFont,m_titleColor);
	}

	bool Frame::OnMoved(const MouseMovedEvent& event, Vec2 relativeMousePos)
	{
		bool moved = GUI::OnMoved(event, relativeMousePos);
		if (event.IsDragged())
		{
			if (m_holdFrame)
			{
				m_position = m_holdPosition + event.GetPosition();
				m_position.x = m_position.x < 0 ? 0 : m_position.x + m_size.x >= Window::GetWidth() ? Window::GetWidth() - m_size.x : m_position.x;
				m_position.y = m_position.y < 0 ? 0 : m_position.y + m_size.y >= Window::GetHeight() ? Window::GetHeight() - m_size.y : m_position.y;
			}
		}
		return moved;
	}

	GUI* Frame::OnPressed(const MousePressedEvent& event, Vec2 relativeMousePos)
	{
		GUI* pressed = GUI::OnPressed(event, relativeMousePos);
		if (pressed != this)
			return pressed;

		if (event.GetButton() == GLFW_MOUSE_BUTTON_1)
		{
			m_holdFrame = false;
			if (IsInsideFrameHeader(relativeMousePos))
			{
				m_holdPosition = m_position - event.GetPosition();
				m_holdFrame = true;
				return this;
			}
		}
		return NULL;
	}

	GUI* Frame::OnReleased(const MouseReleasedEvent& event, Vec2 relativeMousePos)
	{
		GUI::OnReleased(event, relativeMousePos);
		if (event.GetButton() == GLFW_MOUSE_BUTTON_1)
		{
			m_holdFrame = false;
		}
		return NULL;
	}

	bool Frame::IsInsideFrameHeader(const Vec2& mouse) const
	{
		return MOUSE_INSIDE(mouse,0,0,m_size.x,m_margin.top-10);
	}
}
