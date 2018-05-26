#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <math/Vec4.h>
#include <math/Vec2.h>
#include <internal/GreetTypes.h>
#include <iostream>


namespace Greet{ namespace Math {

	inline float RoundDown(float numToRound, float multiple)
	{
		if (multiple == 0)
			return 0;
		return ((int)(numToRound / multiple))*multiple;
	}

	inline float RoundUp(float numToRound, float multiple)
	{
		if (multiple == 0)
			return 0;
		float ans = ((int)(numToRound / multiple))*multiple;
		return ans < numToRound ? ans + multiple : ans;
	}

	inline float RoundClose(float numToRound, float multiple)
	{
		if (multiple == 0)
			return numToRound;
		float down = ((int)(numToRound / multiple))*multiple;
		float up = down < numToRound ? down + multiple : down;
		return (numToRound - down) < (up - numToRound) ? down : up;
	}

	inline bool IsPositive(float val)
	{
		return -val < val;
	}

	//struct Vec2;
	inline void Clamp(float* value, float min, float max)
	{
		if(min < max)
			*value = *value < min ? min : (*value > max ? max : *value);
		else
			*value = *value < max ? max : (*value > min ? min : *value);
	}

	// Returns half the value, add one to value if it is odd.
	inline int Half(int value)
	{
		return (value + (value % 2)) / 2;
	}


	inline float ToRadians(float degrees)
	{
		return (float)(degrees * M_PI * 0.005555555f);
	}

	inline float ToDegrees(float radians)
	{
		return (float)(radians * 180.0f / M_PI);
	}
}}