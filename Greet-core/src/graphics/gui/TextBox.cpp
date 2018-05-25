#include "TextBox.h"


namespace Greet
{

	TextBox::TextBox(const Vec2& position, const Vec2& size, bool password)
		:GUI(position,size, LTRB(3,2,3,2)), m_font(FontManager::Get("anonymous", size.y-6)), m_password(password), m_cursorColor(ColorUtils::GetMaterialColorAsHSV(120 / 360.0f, 9)),
		m_textColor(ColorUtils::GetMaterialColorAsHSV(120 / 360.0f, 3)),	m_blinkSpeed(0.5f)
	{
		m_renderBackground = true;
		m_backgroundColor = ColorUtils::GetMaterialColorAsHSV(120 / 360.0f, 15);
	}

	void TextBox::Submit(GUIRenderer* renderer) const
	{
		if (m_password)
			renderer->SubmitString(std::string(m_text.size(), '*'), Vec2(m_margin.left, m_font->GetSize()), m_font, m_textColor);
		else
			renderer->SubmitString(m_text, Vec2(m_margin.left, m_font->GetSize()), m_font, m_textColor);

		const Vec2& pos = GetRealPosition();
		if (IsFocused())
		{
			if (m_timer < m_blinkSpeed)
			{
				float width = m_font->GetWidthOfText(m_text,0,m_cursorPosition);
				renderer->SubmitRect(Vec2(m_margin.left+width-1, m_margin.top), Vec2(1, m_size.y - (m_margin.top+ m_margin.bottom)), m_cursorColor);
			}
		}
	}

	bool TextBox::Update(float timeElapsed)
	{
		m_timer += timeElapsed;
		if (m_timer >= m_blinkSpeed*2)
			m_timer = 0;
		return GUI::Update(timeElapsed);
	}

	void TextBox::AddCharacter(char c)
	{
		AddCharacter(c, m_cursorPosition);
	}

	void TextBox::AddCharacter(char c, uint index)
	{
		if (index > m_text.size())
			index = m_text.size();
		m_text.insert(m_text.begin()+index,c);
	}


	void TextBox::OnFocused(bool focused)
	{
		m_timer = 0;
	}

	bool TextBox::OnPressed(const KeyPressedEvent& e)
	{
		if (e.GetButton() == GLFW_KEY_BACKSPACE)
		{
			if (m_cursorPosition > 0)
			{
				m_text.erase(m_text.begin() + --m_cursorPosition);
			}
		}
		else if (e.GetButton() == GLFW_KEY_LEFT)
		{
			if (m_cursorPosition > 0)
				m_cursorPosition--;
		}
		else if (e.GetButton() == GLFW_KEY_RIGHT)
		{
			if (m_cursorPosition < m_text.size())
				m_cursorPosition++;
		}
		else if (e.GetButton() == GLFW_KEY_END)
		{
			m_cursorPosition = m_text.size();
		}
		else if (e.GetButton() == GLFW_KEY_HOME)
		{
			m_cursorPosition = 0;
		}
		else if (e.GetButton() == GLFW_KEY_DELETE)
		{
			if (m_cursorPosition < m_text.size())
			{
				m_text.erase(m_text.begin() + m_cursorPosition);
			}
		}
		m_timer = 0;
		return false;
	}

	bool TextBox::OnReleased(const KeyReleasedEvent& e)
	{
		return false;
	}

	bool TextBox::OnTyped(const KeyTypedEvent& e)
	{
		AddCharacter(e.GetCharCode());
		if (m_font->GetWidthOfText(m_text) > m_size.x - m_margin.left - m_margin.right)
			m_text.erase(m_text.end() - 1);
		else
			m_cursorPosition++;
		m_timer = 0;
		return true;
	}

	GUI* TextBox::OnPressed(const MousePressedEvent& e, Vec2 relativeMousePos)
	{
		GUI* gui = GUI::OnPressed(e, relativeMousePos);
		// Check if mouse is inside of the textbox
		if (gui != this)
			return gui;

		float x = relativeMousePos.x - m_margin.left;

		float* widths = m_font->GetPartialWidths(m_text);

		int i = 1;
		while (x > widths[i] && i <=m_text.size())
		{
			i++;
		}

		m_cursorPosition = i - 1;


		return this;
	}

}
