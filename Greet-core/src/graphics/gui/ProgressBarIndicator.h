#pragma once

#include <graphics/gui/GUI.h>

namespace Greet
{
	class ProgressBarIndicator
	{
	public:
		virtual void Submit(GUIRenderer* renderer, const Vec2& size, float percentage, bool vertical) const = 0;
	};
}