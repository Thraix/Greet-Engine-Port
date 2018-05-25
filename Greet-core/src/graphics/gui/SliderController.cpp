#include "SliderController.h"

namespace Greet
{

	SliderController::SliderController(const Vec2& position, const Vec2& size)
		: GUI(position, size, LTRB(0, 0, 0, 0))
	{
		SetBackgroundColor(ColorUtils::GetMaterialColorAsHSV(120 / 360.0f, 9));
	}

	void SliderController::Begin(GUIRenderer* renderer) const 
	{
		renderer->PushMatrix(m_transformationMatrix);
		renderer->PushViewport(Vec2(-Math::Half(m_size.x), -Math::Half(m_size.y)), m_size, true);
		if (m_renderBackground)
			RenderBackground(renderer);
	}

	void SliderController::RenderBackground(GUIRenderer* renderer) const
	{
		renderer->SubmitRect(Vec2(-Math::Half(m_size.x),-Math::Half(m_size.y)), m_size, m_backgroundColor);
	}

	bool SliderController::OnMoved(const MouseMovedEvent& event, Vec2 relativeMousePos)
	{
		// Check if mouse is inside the controller
		GUI::OnMoved(event, relativeMousePos);
		bool inside = m_mouseInside;
		if (inside && !m_wasInside)
		{
			// Dummy variables
			float f0, f1;
			DriverDispatcher::AddDriver(new RectDriver(f0, f1, m_size.x, m_size.y, -3.0f, -3.0f, 6.0f, 6.0f, 0.08f, true, new DriverAdapter()));
		}
		else if (!inside && m_wasInside)
		{
			// Dummy variables
			float f0, f1;
			DriverDispatcher::AddDriver(new RectDriver(f0, f1, m_size.x, m_size.y, 3.0f, 3.0f, -6.0f, -6.0f, 0.08f, true, new DriverAdapter()));
		}
		m_wasInside = inside;
		return false;
	}

	bool SliderController::IsInside(const Vec2& position) const
	{
		return position.x >= -Math::Half(m_size.x) && position.x < m_size.x - Math::Half(m_size.x) && position.y >= -Math::Half(m_size.y) && position.y < m_size.y - Math::Half(m_size.y);
	}
}