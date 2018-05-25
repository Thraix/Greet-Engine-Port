#pragma once

#include <math/MathFunc.h>
#include <graphics/gui/GUI.h>
#include <graphics/gui/SliderController.h>
#include <utils/ColorUtils.h>
#include <graphics/fonts/FontManager.h>
#include <drivers/RectDriver.h>
#include <drivers/DriverDispatcher.h>
#include <graphics/layers/GUILayer.h>

namespace Greet {

	class Slider : public GUI
	{
	private:

		bool m_vertical;

		// True if the slider should snap into position
		bool m_snap;

		// Size of the steps in between snap values
		float m_stepSize;
		// Value when the slider is at the start
		float m_startValue;
		// Value when the slider is at the end
		float m_endValue;
		// True if the label should be displayed as a percentage.
		bool m_percentage;

		// True if the user is holding the slider
		bool m_holdSlider;
		// Slider label font
		Font* m_sliderFont;
		
		// The slidercontroller for this slider
		// Will get deleted by the engine
		SliderController* m_sliderController;

		// Defined labels for the slider
		// If empty display the value instead
		std::vector<std::string> m_labels;
	private:
		void SnapSlider();
	public:
		Slider(const Vec2& position, const Vec2& size, float start, float end, float stepSize, bool vertical = false, float sliderWidth = 7);
		Slider(const Vec2& position, const Vec2& size, const std::vector<std::string>& labels, bool vertical = false, float sliderWidth = 7);
		virtual ~Slider();

		void Submit(GUIRenderer* renderer) const override;
		void End(GUIRenderer* renderer) const override;
		bool OnMoved(const MouseMovedEvent& event, Vec2 relativeMousePos) override;
		GUI* OnPressed(const MousePressedEvent& event, Vec2 relativeMousePos) override;
		GUI* OnReleased(const MouseReleasedEvent& event, Vec2 relativeMousePos) override;

		void SetSnapping(bool snapping);
		bool IsInside(const Vec2& position) const override;
		void SetRenderPercentage(bool percentage);
		void SetStepSize(float stepSize) { if(m_labels.empty()) m_stepSize = stepSize; }
		void SetSliderController(SliderController* controller);
		void SetValue(float value);
		inline float GetValue() const;
		inline float GetPosFromValue() const;
		inline float GetPosFromValue(float value) const;
		inline float GetStepSize() const { return m_stepSize; }
		inline float GetSliderPos() const { return m_sliderController->m_position.x; }

	};
}
