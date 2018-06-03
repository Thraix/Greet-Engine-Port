#include "Joystick.h"

#include <logging/Log.h>

namespace Greet {
	Joystick::Joystick(uint jsNum, float calibrateLeft, float calibrateRight)
		: m_jsNum(jsNum), m_calibrateLeft(calibrateLeft), m_calibrateRight(calibrateRight)
	{
		ClearInput();
		m_connected = false;
		m_wasConnected = false;
		for (uint i = 0; i < GLFW_JOYSTICK_BUTTONS; i++)
		{
			m_mapping.push_back(i);
		}
	}

	void Joystick::Update()
	{
		m_wasConnected = m_connected;
		if (m_connected)
		{
			int count;
			const float* axes = glfwGetJoystickAxes(m_jsNum, &count);
			if (count == 0)
			{
				m_connected = false;
				ClearInput();
				return;
			}
			ASSERT((count == GLFW_JOYSTICK_AXES), "CONTROLLER NOT SUPPORTED. INVALID AMOUNT OF AXES, found ", count, ", wanted ", GLFW_JOYSTICK_AXES, ".");
			m_leftStick = Vec2(axes[0], axes[1]);
			if (m_leftStick.Length()<m_calibrateLeft)
				m_leftStick = Vec2(0, 0);

			m_rightStick = Vec2(axes[3], axes[4]);
			if (m_rightStick.Length()<m_calibrateRight)
				m_rightStick = Vec2(0, 0);

			const unsigned char* buttons = glfwGetJoystickButtons(m_jsNum,&count);
			if (count == 0)
			{
				m_connected = false;
				ClearInput();
				return;
			}
			ASSERT((count == GLFW_JOYSTICK_BUTTONS), "CONTROLLER NOT SUPPORTED. INVALID AMOUNT OF BUTTONS, found ", count, ", wanted ", GLFW_JOYSTICK_BUTTONS, ".");

			for (int i = 0; i < GLFW_JOYSTICK_BUTTONS;i++)
			{
				buttonPas[i] = buttonCur[i];
				buttonCur[i] = buttons[i]==GLFW_PRESS;
			}
		}
	}

	void Joystick::RemapButton(uint button, uint rebind)
	{
		
		uint b = m_mapping[button];
		for (int i = 0; i < GLFW_JOYSTICK_BUTTONS; i++)
		{
			if (m_mapping[i] == rebind)
			{
				m_mapping[i] = button;
				break;
			}
		}
		m_mapping[button] = rebind;
	}
	bool Joystick::CheckConnect()
	{
		m_wasConnected = m_connected;
		m_connected = glfwJoystickPresent(m_jsNum);
		return m_connected;
	}

	void Joystick::ClearInput()
	{
		memset(buttonPas, false, GLFW_JOYSTICK_BUTTONS);
		memset(buttonCur, false, GLFW_JOYSTICK_BUTTONS);
		m_leftStick = Vec2(0, 0);
		m_rightStick = Vec2(0, 0);
	}

	bool Joystick::ButtonExists(uint buttoncode) const
	{
		if (buttoncode >= GLFW_JOYSTICK_BUTTONS)
		{
			Log::Error("Joystick button could not be found: ", buttoncode);
			return false;
		}
		return true;
	}

	bool Joystick::IsButtonPressed(uint buttoncode) const
	{
		buttoncode = m_mapping[buttoncode];
		if (ButtonExists(buttoncode))
			return buttonCur[buttoncode] && !buttonPas[buttoncode];
		return false;
	}

	bool Joystick::IsButtonReleased(uint buttoncode) const
	{
		buttoncode = m_mapping[buttoncode];
		if (ButtonExists(buttoncode))
			return !buttonCur[buttoncode] && buttonPas[buttoncode];
		return false;
	}

	bool Joystick::IsButtonDown(uint buttoncode) const
	{
		buttoncode = m_mapping[buttoncode];
		if (ButtonExists(buttoncode))
			return buttonCur[buttoncode];
		return false;
	}

	bool Joystick::IsRealButtonPressed(uint buttoncode) const
	{
		if (ButtonExists(buttoncode))
			return buttonCur[buttoncode] && !buttonPas[buttoncode];
		return false;
	}

	bool Joystick::IsRealButtonReleased(uint buttoncode) const
	{
		if (ButtonExists(buttoncode))
			return !buttonCur[buttoncode] && buttonPas[buttoncode];
		return false;
	}

	bool Joystick::IsRealButtonDown(uint buttoncode) const
	{
		if (ButtonExists(buttoncode))
			return buttonCur[buttoncode];
		return false;
	}
}
