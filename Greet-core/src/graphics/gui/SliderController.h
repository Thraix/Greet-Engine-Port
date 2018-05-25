#pragma once

#include <graphics/gui/GUI.h>
#include <drivers/RectDriver.h>
#include <drivers/DriverDispatcher.h>
#include <drivers/DriverAdapter.h>
#include <graphics/layers/GUILayer.h>

namespace Greet {

	class SliderController : public GUI
	{
	private:
		bool m_wasInside;

	public:
		SliderController(const Vec2& position, const Vec2& size);

		virtual bool IsInside(const Vec2& position) const override;
		virtual void Begin(GUIRenderer* renderer) const override;
		virtual void RenderBackground(GUIRenderer* renderer) const override;
		virtual void Submit(GUIRenderer* renderer) const override {}

		bool OnMoved(const MouseMovedEvent& event, Vec2 relativeMousePos) override;
	};

};