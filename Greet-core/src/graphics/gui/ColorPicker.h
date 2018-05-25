#pragma once

#include <graphics/gui/GUI.h>
#include <graphics/gui/Slider.h>
#include <graphics/gui/SliderController.h>

#define TAN30 0.57735026919

namespace Greet
{

	class HueSliderController : public SliderController
	{
	private:
		float m_arrowSize;
		float m_sliderWidth;
	public:
		HueSliderController(const Vec2& pos, float sliderWidth, float arrowSize)
			: SliderController(pos, Vec2(sliderWidth+arrowSize*2, 1 + 2*TAN30*arrowSize)),
			m_arrowSize(arrowSize), m_sliderWidth(sliderWidth)
		{
					
		};

		void RenderBackground(GUIRenderer* renderer) const override {
			Vec4 color = Vec4(0, 0, 0.5, 1);
			Vec2 pos1 = Vec2(-Math::Half(m_size.x), -Math::Half(m_size.y));
			Vec2 pos2 = Vec2(Math::Half(m_size.x), -Math::Half(m_size.y));

			renderer->SubmitTriangle(pos1, Vec2(pos1.x+m_arrowSize, 0), Vec2(pos1.x, 0), color, true);
			renderer->SubmitLine(Vec2(pos1.x, 0), m_arrowSize, 1, false, color, true);
			renderer->SubmitTriangle(Vec2(pos1.x, -pos1.y + 1), Vec2(pos1.x, 1), Vec2(pos1.x + m_arrowSize, 1), color, true);

			renderer->SubmitTriangle(pos2, Vec2(pos2.x - m_arrowSize, 0), Vec2(pos2.x, 0), color, true);
			renderer->SubmitLine(Vec2(pos2.x, 0), -m_arrowSize, 1, false, color, true);
			renderer->SubmitTriangle(Vec2(pos2.x, -pos2.y + 1), Vec2(pos2.x, 1), Vec2(pos2.x - m_arrowSize, 1), color, true);
		}

		bool OnMoved(const MouseMovedEvent& event, Vec2 relativeMousePos) override { return GUI::OnMoved(event, relativeMousePos); }
	};

	class HueSlider : public Slider
	{
		friend class ColorPicker;
	public:
		HueSlider(const Vec2& position, const Vec2& size);
		void RenderBackground(GUIRenderer* renderer) const override;
	};

	class ColorPicker : public GUI
	{
	private:
		float m_pickerSize;
		float m_pickerWidth;
		float m_spacing;
		bool m_holding;
		Vec2 m_cursorPos;
		HueSlider* m_hueSlider;
	public:
		ColorPicker(const Vec2& position, float pickerSize, float pickerWidth, float spacing);

		GUI* OnPressed(const MousePressedEvent& event, Vec2 relativeMousePos) override;
		GUI* OnReleased(const MouseReleasedEvent& event, Vec2 relativeMousePos) override;
		bool OnMoved(const MouseMovedEvent& event, Vec2 relativeMousePos) override;
		void SetColor(const Vec4& hsvColor);
		void SetColorAsRGB(const Vec4& rgbColor);
		Vec4 GetColor() const;
		void Submit(GUIRenderer* renderer) const override;
	};
	
}