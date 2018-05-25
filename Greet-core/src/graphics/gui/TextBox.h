#pragma once

#include <graphics/gui/GUI.h>
#include <graphics/fonts/Font.h>
#include <graphics/fonts/FontManager.h>
#include <graphics/Window.h>

namespace Greet
{
	class TextBox : public GUI
	{
	protected:
		// Blinks the cursor
		float m_timer;
		// Blinking speed of the cursor
		float m_blinkSpeed;
		// Color of the cursor
		Vec4 m_cursorColor;
		// Color of the text
		Vec4 m_textColor;
		// Font of the text
		Font* m_font;
		// Text that is entered
		std::string m_text;
		// Cursor position
		uint m_cursorPosition;
		// true if characters should be hidden
		bool m_password;
	public:

		TextBox(const Vec2& position, const Vec2& size, bool password = false);

		bool Update(float timeElapsed) override;
		void Submit(GUIRenderer* renderer) const override;

		void AddCharacter(char c);
		void AddCharacter(char c, uint index);

		void SetText(const std::string& text) { m_text = text;m_cursorPosition = text.size()-1; }
		void SetTextColor(const Vec4& color) { m_textColor = color; }
		void SetCursorColor(const Vec4& color) { m_cursorColor = color; }
		void SetBlinkingSpeed(float time) { m_blinkSpeed = time; }
		const std::string& GetText() const { return m_text; }
		const Vec4& GetTextColor() const { return m_textColor; }
		const Vec4& GetCursorColor() const { return m_cursorColor; }
		float GetBlinkingSpeed() const { return m_blinkSpeed; }

		bool OnPressed(const KeyPressedEvent& e) override;
		bool OnReleased(const KeyReleasedEvent& e) override;
		bool OnTyped(const KeyTypedEvent& e) override;

		virtual GUI* OnPressed(const MousePressedEvent& event, Vec2 relativeMousePos) override;

		void OnFocused(bool focused);

	};
}