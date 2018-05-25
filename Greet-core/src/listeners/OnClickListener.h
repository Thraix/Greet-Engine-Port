#pragma once

#include <math/Maths.h>
namespace Greet {
	class GUI;
	struct OnClickListener
	{
	public:
		virtual void OnClick(GUI* gui) = 0;
	};
}