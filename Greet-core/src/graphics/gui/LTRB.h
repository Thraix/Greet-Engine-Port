#pragma once

namespace Greet {

	struct LTRB
	{
		float top;
		float left;
		float bottom;
		float right;

		LTRB()
			: left(0), top(0), right(0), bottom(0)
		{
		
		}

		LTRB(float left, float top, float right, float bottom) 
			: left(left), top(top), right(right), bottom(bottom)
		{
		
		}
	};

}