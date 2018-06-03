#pragma once

#include "GUIMouseListener.h"
#include "GUIUtils.h"
#include <graphics/renderers/GUIRenderer.h>
#include <event/MouseEvent.h>
#include <event/KeyEvent.h>

#include <vector>

namespace Greet {
	class Content
	{
	protected:
		bool m_isFocusable;
		std::vector<Content*> m_contents;
		Content* m_focused;
		bool isFocused;
		float marginLeft;
		float marginRight;
		float marginTop;
		float marginBottom;
		XMLObject xmlObject;

		float ySpacing;

		Vec2 size;
		Content* parent;
		Vec4 backgroundColor;

	public:
		Content();
		Content(const XMLObject& object, Content* parent);

		virtual void Render(GUIRenderer* renderer, const Vec2& position) const;
		virtual void Update(float timeElapsed);

		void AddContent(Content* content);
		Content* RemoveContent(uint index);
		Content* RemoveContent(Content* content);
		Content* GetContent(uint index);

		// Returns true if this component or a child that was pressed is focusable.
		bool MousePress(const MousePressedEvent& event, const Vec2& translatedPos, const GUIMouseListener& listener);
		void MouseRelease(const MouseReleasedEvent& event, const Vec2& translatedPos, const GUIMouseListener& listener);
		void MouseMove(const MouseMovedEvent& event, const Vec2& translatedPos);

		//virtual void OnMousePressed(const MousePressedEvent& event, const Vec2& translatedPos);
		//virtual void OnMouseReleased(const MouseReleasedEvent& event, const Vec2& translatedPos);
		//virtual void OnMouseClicked(const MouseReleasedEvent& event, const Vec2& translatedPos);
		//virtual void OnMouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos);
		//virtual void OnMouseEntered();
		//virtual void OnMouseExited();
		virtual void OnKeyPressed(const KeyPressedEvent& event);
		virtual void OnKeyReleased(const KeyReleasedEvent& event);


		virtual void OnFocused();
		virtual void OnUnfocused();
		virtual bool IsFocusable() const { return m_isFocusable; };

		void SetSpacing(float spacing);
		float GetSpacing() const;
		bool IsMouseInside(const Vec2& mousePos) const;
		Vec2 GetSize() const;
		void SetSize(const Vec2& size);
		virtual float GetWidth() const;
		virtual float GetHeight() const;
		virtual float GetPotentialWidth() const;
		virtual float GetPotentialHeight() const;

		void SetMargins(float left, float right, float top, float bottom);
	};
}
