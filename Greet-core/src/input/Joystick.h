#pragma once

#include <vector>
#include <internal/GreetTypes.h>
#include <math/Maths.h>
#include <internal/GreetGL.h>

#ifndef MAX_JOYSTICKS 
	#define MAX_JOYSTICKS 4
#endif


namespace Greet {

	class Joystick
	{

		friend class Window;

	private:
		std::vector<uint> m_mapping;
		bool m_connected = false;
		bool m_wasConnected = false;
		//STORES THE JOYSTICK NUMBER
		const int m_jsNum;

		float m_calibrateLeft;
		float m_calibrateRight;

		//AXES
		Vec2 m_leftStick;
		Vec2 m_rightStick;

		bool buttonCur[GLFW_JOYSTICK_BUTTONS];
		bool buttonPas[GLFW_JOYSTICK_BUTTONS];

	private:
		Joystick(uint jsNum, float m_calibrateLeft, float m_calibrateRight);
		void Update();
		bool CheckConnect();
		void ClearInput();
	public:

		inline Vec2 GetLeftStick() const { return m_leftStick; }
		inline Vec2 GetRightStick() const { return m_rightStick; }

		void RemapButton(uint button, uint rebind);
		bool ButtonExists(uint buttoncode) const;
		bool IsButtonPressed(uint buttoncode) const;
		bool IsButtonReleased(uint buttoncode) const;
		bool IsButtonDown(uint buttoncode) const;
		bool IsRealButtonPressed(uint buttoncode) const;
		bool IsRealButtonReleased(uint buttoncode) const;
		bool IsRealButtonDown(uint buttoncode) const;
		bool IsConnected() const { return m_connected; }

	};
}
