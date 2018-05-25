#pragma once

#include <graphics/Window.h>
#include <utils/Timer.h>
#include <logging/Log.h>
#include <graphics/RenderEngine.h>

namespace Greet {

	class App : public WindowListener, public JoystickStateListener
	{
	protected:
		Timer* m_timer;
		uint m_fps;
		uint m_ups;
		double frameCap = 0;
		bool m_initialized = false;
	protected:
		App();

		virtual ~App();

		void CreateWindow(std::string title, int width, int height);


		virtual void Init() = 0;
		virtual void Tick() = 0;
		virtual void Update(float elapsedTime) = 0;
		virtual void Render() = 0;
		virtual void WindowResize(int width, int height){}
		virtual void JoystickState(uint joystick, bool connect){}
		virtual void WindowFocus(bool focused) {}

		void SetFrameCap(uint frameCap)
		{
			if (frameCap == 0)
				this->frameCap = 0;
			else
				this->frameCap = 1.0 / (double)frameCap;
		}

		uint GetFrameCap()
		{
			if (frameCap == 0)
				return 0;
			return (uint)(1.0 / frameCap);
		}

		const uint GetFPS() const { return m_fps; }
		const uint GetUPS() const { return m_ups; }

	public:
		void Start();

	private:
		void Run();
	};
}
