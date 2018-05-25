#pragma once

#include "Event.h"
#include <internal/GreetTypes.h>
#include <math/Maths.h>

namespace Greet {

	class MouseButtonEvent : public Event
	{
	protected:
		Vec2 m_position;
		uint m_button;
	protected:
		MouseButtonEvent(float x, float y, uint button, EventType type)
			: Event(type),m_position(x,y),m_button(button){}
	public:
		inline const float GetX() const { return m_position.x; }
		inline const float GetY() const { return m_position.y; }
		inline const void SetX(float x) { m_position.x = x; }
		inline const void SetY(float y) { m_position.y = y; }
		inline const Vec2& GetPosition() const { return m_position; }
		inline const uint GetButton() const { return m_button; }
	};

	class MousePressedEvent : public MouseButtonEvent
	{
	public:
		MousePressedEvent(float x, float y, uint button)
			: MouseButtonEvent(x, y, button, MOUSE_PRESSED) {}
	};

	class MouseReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseReleasedEvent(float x, float y, uint button)
			: MouseButtonEvent(x, y, button, MOUSE_RELEASED) {}
	};

	class MouseMovedEvent : public Event
	{
	private:
		Vec2 m_position;
		Vec2 m_deltaPosition;
		bool m_dragged;

	public:
		MouseMovedEvent(float x, float y, float dx, float dy, bool dragged)
			:Event(MOUSE_MOVED),m_position(Vec2(x,y)), m_deltaPosition(Vec2(dx,dy)),m_dragged(dragged){}
	public:
		inline const float GetX() const { return m_position.x; }
		inline const float GetY() const { return m_position.y; }
		inline const float GetDX() const { return m_deltaPosition.x; }
		inline const float GetDY() const { return m_deltaPosition.y; }
		inline const void SetX(float x) { m_position.x = x; }
		inline const void SetY(float y) { m_position.y = y; }
		inline const Vec2& GetPosition() const { return m_position; }
		inline const Vec2& GetDeltaPosition() const { return m_deltaPosition; }
		inline const bool IsDragged() const { return m_dragged; }
	};

	class MouseScrollEvent : public Event
	{
	private:
		int m_scroll;
		int m_scrollDirection;
	public:
		MouseScrollEvent(int scroll)
			//															Calculate the direction, keeping in mind div by 0
			:Event(MOUSE_SCROLLED), m_scroll(scroll), m_scrollDirection(scroll / (scroll < 0 ? -scroll : (scroll == 0 ? 1 : scroll))) {}

		inline const int GetScroll() const { return m_scroll; };
		inline const int GetDirection() const { return m_scrollDirection; }

	};
}