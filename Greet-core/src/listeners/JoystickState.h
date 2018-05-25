#pragma once

#include <internal/GreetTypes.h>

namespace Greet {

	class JoystickStateListener
	{
	public:
		virtual void JoystickState(uint joystickIndex, bool state) = 0;
	};
}
