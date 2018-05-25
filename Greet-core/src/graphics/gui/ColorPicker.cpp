#include "ColorPicker.h"

namespace Greet
{

	HueSlider::HueSlider(const Vec2& position, const Vec2& size)
		:Slider(position,size,0,0.999f,0,true)
	{
		SetSliderController(new HueSliderController(size/2,size.x,10));
	}

	void HueSlider::RenderBackground(GUIRenderer* renderer) const
	{
		renderer->SubmitRect(Vec2(0, 0), m_size, Vec4(0, 1, 1, 1), Vec4(0, 1, 1, 1), Vec4(1, 1, 1, 1), Vec4(1, 1, 1, 1));
	}

	ColorPicker::ColorPicker(const Vec2& position, float pickerSize, float pickerWidth, float spacing)
		: GUI(position, Vec2(pickerSize + spacing*2 + pickerWidth+100, pickerSize)),
		m_pickerSize(pickerSize), m_pickerWidth(pickerWidth), m_spacing(spacing)
	{
		m_renderBackground = false;
		m_hueSlider = new HueSlider(Vec2(m_pickerSize + m_spacing, 0), Vec2(m_pickerWidth, m_pickerSize));
		Add(m_hueSlider);
		SetColor(Vec4(1.0f, 0.0f, 1.0f, 1));
	}

	GUI* ColorPicker::OnPressed(const MousePressedEvent& event, Vec2 relativeMousePos)
	{
		GUI* pressed = GUI::OnPressed(event, relativeMousePos);
		if (pressed == this)
		{
			if (GUI::IsInside(Vec2(0, 0), Vec2(m_pickerSize, m_pickerSize), relativeMousePos))
			{
				m_holding = true;
				m_cursorPos = relativeMousePos;
				Math::Clamp(&m_cursorPos.x, 0, m_pickerSize - 1);
				Math::Clamp(&m_cursorPos.y, 0, m_pickerSize - 1);
				return this;
			}
		}
		// We didn't press anything
		return NULL;
	}

	GUI* ColorPicker::OnReleased(const MouseReleasedEvent& event, Vec2 relativeMousePos)
	{
		m_holding = false;
		return GUI::OnReleased(event, relativeMousePos);
	}

	bool ColorPicker::OnMoved(const MouseMovedEvent& event, Vec2 relativeMousePos)
	{
		bool inside = GUI::OnMoved(event, relativeMousePos);
		if (inside && event.IsDragged() && m_holding)
		{
				m_cursorPos = relativeMousePos;
				Math::Clamp(&m_cursorPos.x, 0, m_pickerSize - 1);
				Math::Clamp(&m_cursorPos.y, 0, m_pickerSize - 1);
				return true;
		}
		else if (m_hueSlider->IsInside(TranslateMouse(relativeMousePos, m_hueSlider)))
		{
			return true;
		}
		return inside;
	}

	void ColorPicker::Submit(GUIRenderer* renderer) const
	{
		float hue = m_hueSlider->GetValue();
		renderer->SubmitRect(Vec2(0, 0), Vec2(m_pickerSize, m_pickerSize), Vec4(hue, 1, 0, 1), Vec4(hue, 1, 1, 1), Vec4(hue, 0, 0, 1), Vec4(hue, 0, 1, 1));
		renderer->SubmitRect(Vec2(m_pickerSize + m_spacing * 2 + m_pickerWidth, 0), Vec2(100, 50), GetColor(), true);

		renderer->SubmitLine(m_cursorPos + Vec2(-3, -3), 7, 2, false, Vec4(0, 0, 0.75, 1.0), true);
		renderer->SubmitLine(m_cursorPos + Vec2(-3, 2), 7, 2, false, Vec4(0, 0, 0.75, 1.0), true);
		renderer->SubmitLine(m_cursorPos + Vec2(-3, -3), 7, 2, true, Vec4(0, 0, 0.75, 1.0), true);
		renderer->SubmitLine(m_cursorPos + Vec2(2, -3), 7, 2, true, Vec4(0, 0, 0.75, 1.0), true);
	}
	
	void ColorPicker::SetColor(const Vec4& hsvColor)
	{
		m_hueSlider->SetValue(hsvColor.x);
		m_cursorPos = Vec2(hsvColor.z, 1.0 - hsvColor.y) * (m_pickerSize - 1);
	}

	void ColorPicker::SetColorAsRGB(const Vec4& rgbColor)
	{
		SetColor(ColorUtils::RGBtoHSV(rgbColor));
	}

	Vec4 ColorPicker::GetColor() const
	{
		return Vec4(m_hueSlider->GetValue(), 1.0 - m_cursorPos.y / (m_pickerSize - 1), m_cursorPos.x / (m_pickerSize - 1), 1);
	}
}