#pragma once

namespace Greet {
	class WindowResizeListener
	{
	public:
		virtual void WindowResize(int width, int height) = 0;
	};
}