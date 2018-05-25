#include "Slider.h"

namespace Greet {

	Slider::Slider(const Vec2& position, const Vec2& size, float start, float end, float stepSize, bool vertical, float sliderWidth)
		: GUI(position, Vec2(size.x, size.y)), m_stepSize(stepSize), m_startValue(start), m_endValue(end), m_snap(false),
		m_sliderFont(FontManager::Get("anonymous",24)), m_sliderController(new SliderController(size/2, Vec2(sliderWidth, size.y+6))),
		m_vertical(vertical), m_holdSlider(false)
	{
		if (m_vertical)
		{
			m_sliderController->m_size = Vec2(size.x + 6, sliderWidth);
			m_sliderController->m_position.y = GetPosFromValue();
		}
		else
		{
			m_sliderController->m_position.x = GetPosFromValue();
		}
		m_renderBackground = true;
		m_backgroundColor = ColorUtils::GetMaterialColorAsHSV(120 / 360.0f, 15);
		Add(m_sliderController);
	}

	Slider::Slider(const Vec2& position, const Vec2& size, const std::vector<std::string>& labels, bool vertical, float sliderWidth)
		: Slider(position, size, 0,labels.size()-1,1,vertical, sliderWidth)
	{
		m_labels = labels;
		SetSnapping(true);
	}

	Slider::~Slider()
	{
		delete m_sliderController;
	}

	bool Slider::OnMoved(const MouseMovedEvent& event, Vec2 relativeMousePos)
	{
		bool moved = GUI::OnMoved(event, relativeMousePos);
		if (event.IsDragged())
		{
			if (m_holdSlider)
			{
				if (m_vertical)
				{
					m_sliderController->m_position.y = relativeMousePos.y;
					Math::Clamp(&m_sliderController->m_position.y, 0, m_size.y - 1);
				}
				else
				{
					m_sliderController->m_position.x = relativeMousePos.x;
					Math::Clamp(&m_sliderController->m_position.x, 0, m_size.x - 1);
				}
				if(m_snap)
					SnapSlider();
			}
		}
		return moved;
	}

	GUI* Slider::OnPressed(const MousePressedEvent& event, Vec2 relativeMousePos)
	{
		GUI* pressed = GUI::OnPressed(event, relativeMousePos);
		if (pressed != this && pressed != m_sliderController)
			return pressed;
		if (event.GetButton() == GLFW_MOUSE_BUTTON_1)
		{
			m_holdSlider = true;
			if (m_vertical)
			{
				m_sliderController->m_position.y = relativeMousePos.y;
				Math::Clamp(&m_sliderController->m_position.y, 0, m_size.y - 1);
			}
			else
			{
				m_sliderController->m_position.x = relativeMousePos.x;
				Math::Clamp(&m_sliderController->m_position.x, 0, m_size.x - 1);
			}
			if (m_snap)
				SnapSlider();
			return pressed;
		}

		// If we didn't press on the slider return NULL
		return NULL;
	}

	GUI* Slider::OnReleased(const MouseReleasedEvent& event, Vec2 relativeMousePos)
	{
		GUI::OnReleased(event, relativeMousePos);
		if (event.GetButton() == GLFW_MOUSE_BUTTON_1)
		{
			m_holdSlider = false;
		}
		return NULL;
	}

	void Slider::Submit(GUIRenderer* renderer) const
	{
		
	}

	void Slider::End(GUIRenderer* renderer) const
	{
		GUI::End(renderer);
		renderer->PushMatrix(m_transformationMatrix);
		Vec4 sliderColor = ColorUtils::GetMaterialColorAsHSV(120 / 360.0f, 3);
		// Start and end values
		std::string value;
		if (m_labels.size() == 0)
		{
			if (m_percentage)
				value = StringUtils::to_string((int)(GetValue() / (m_endValue - m_startValue) * 100)) + "%";
			else
				value = StringUtils::to_string(GetValue());
		}
		else
		{
			value = m_labels[GetValue()];
		}
		if (!m_vertical)
		{
			float yPos = m_size.y / 2.0f + m_sliderFont->GetSize()*0.25f;
			renderer->SubmitString(value, Vec2((m_size.x - m_sliderFont->GetWidthOfText(value)) / 2.0f, yPos), m_sliderFont, sliderColor);
		}
		renderer->PopMatrix();
	}

	void Slider::SnapSlider()
	{
		if(m_vertical)
			m_sliderController->m_position.y = GetPosFromValue();
		else
			m_sliderController->m_position.x = GetPosFromValue();
	}

	bool Slider::IsInside(const Vec2& position) const
	{
		return GUI::IsInside(position) || m_sliderController->IsInside(TranslateMouse(position, m_sliderController));
	}

	void Slider::SetSnapping(bool snapping)
	{
		m_snap = snapping;
		if(m_snap)
			SnapSlider();
	}

	void Slider::SetRenderPercentage(bool percentage)
	{
		m_percentage = percentage;
	}

	void Slider::SetSliderController(SliderController* controller)
	{
		controller->m_position = m_sliderController->m_position;
		GUI::Remove(m_sliderController);
		delete m_sliderController;
		m_sliderController = controller;
		GUI::Add(controller);
	}

	void Slider::SetValue(float value)
	{
		Math::Clamp(&value, m_startValue, m_endValue);
		if(m_vertical)
			m_sliderController->m_position.y = GetPosFromValue(value);
		else
			m_sliderController->m_position.x = GetPosFromValue(value);
	}

	float Slider::GetValue() const
	{ 
		if(m_vertical)
			return Math::RoundClose((m_sliderController->m_position.y / (m_size.y - 1))*(m_endValue - m_startValue) + m_startValue, m_stepSize);
		else
			return Math::RoundClose((m_sliderController->m_position.x / (m_size.x-1))*(m_endValue - m_startValue) + m_startValue, m_stepSize);
	}

	float Slider::GetPosFromValue(float value) const
	{
		if (m_vertical)
			return (value - m_startValue) / (m_endValue - m_startValue)*(m_size.y - 1);
		else
			return (value - m_startValue) / (m_endValue - m_startValue)*(m_size.x - 1);
	}

	float Slider::GetPosFromValue() const 
	{ 
		return GetPosFromValue(GetValue()); 
	}
}
