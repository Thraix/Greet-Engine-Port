#if 0
#pragma once

#include "Container.h"
#include "Label.h"
#include <Greet.h>
namespace Greet {
	class FrameContainer : public Container
	{
	protected:
		// True if this class created the header.
		bool m_shouldDeleteHeader;

		bool m_movable;
		bool m_moving;

		Content* header;

		float headerSpacing;
	public:
		float headerSize;

	public:
		FrameContainer(const Vec2& pos, const Vec2& size, Content* content, const std::string& title);
		FrameContainer(const Vec2& pos, const Vec2& size, Content* content, Content* header);
		virtual ~FrameContainer();

		void Render(GUIRenderer* renderer) const override;

		// Check if the mouse is within the move window
		bool CheckMove(const Vec2& mousePos);

		// Move the window to the mouse position
		void Move(const Vec2& mousePos);

		// Clamp the container inside window after moving it.
		void MoveScreenClamp();

		// Getters and setters
		Vec2 GetContentPosition() const override { return Vec2(marginLeft, marginTop + headerSize + headerSpacing); };

		// Listeners
		bool OnPressed(const MousePressedEvent& event) override;
		void OnReleased(const MouseReleasedEvent& event) override;
		void OnMoved(const MouseMovedEvent& event) override;
	};
}
#endif
