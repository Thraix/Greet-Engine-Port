#pragma once

#include <graphics/gui/GUI.h>
#include <graphics/fonts/FontManager.h>
#include <graphics/gui/DefaultProgressBarIndicator.h>

namespace Greet {

	class ProgressBar : public GUI
	{
		float m_progress;
		float& m_progressref;
		float m_min;
		float m_max;
		float m_vertical;
		Vec4 m_progressColor;
		Vec4 m_labelColor;
		Font* m_progressFont;
		ProgressBarIndicator* m_indicator;

	public:
		ProgressBar(const Vec2& position, const Vec2& size, float* progress, float min, float max, ProgressBarIndicator* indicator = new DefaultProgressBarIndicator(ColorUtils::GetMaterialColorAsHSV(120 / 360.0f, 9)), bool vertical = false);
		ProgressBar(const Vec2& position, const Vec2& size, float progress, float min, float max, ProgressBarIndicator* indicator = new DefaultProgressBarIndicator(ColorUtils::GetMaterialColorAsHSV(120 / 360.0f, 9)), bool vertical = false);

		void Submit(GUIRenderer* renderer) const override;

		void SetProgress(float* progress);
		void SetProgress(float progress);

		float GetProgress() const { return m_progressref; }
		float GetProgressPercentage() const { return (m_progressref - m_min) / (m_max - m_min); }

		float GetMinValue() const { return m_min; }
		float GetMaxValue() const { return m_max; }

		const Vec4& GetLabelColor() const { return m_labelColor; }
		const Vec4& GetProgressColor() const { return m_progressColor; }

		void SetLabelColor(const Vec4& color) { m_labelColor = color; }
		void SetProgressColor(const Vec4& color) { m_progressColor = color; }

	};
}