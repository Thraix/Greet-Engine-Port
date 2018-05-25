#pragma once

#include <internal/GreetTypes.h>

namespace Greet {
	class Noise
	{

public:

	private:
		Noise() = delete;
		Noise(const Noise& noise) = delete;

	public:
		/*
		* width = width of the noise
		* height = height of the noise
		* octave = number of iterations the noise should go through
		* frequencyX = number of pixels in the first iteration in the x axis
		* frequencyY = number of pixels in the first iteration in the y axis
		*/
		static float* GenNoise(uint width, uint height, uint octave, uint frequencyX, uint frequencyY, float persistance);
	private:
		static float Smooth(uint noiseX, uint noiseY, const float* noise, uint noiseWidth, uint noiseHeight);
	public:
		static float PRNG(int x, int y);
		/*
		   |---| d1
	   --- v1 -------- v2
		|  |			|
	   --- |	x		|
		d2 |			|
		   v3 -------- v4
		*/
		static float Interpolate(float v1, float v2, float v3, float v4, float d1, float d2);
		/*
		* v1 = value of one of the nodes
		* v2 = value of the other node
		* distance of the current position to v1 (between 0 and 1 where 0 is on v1 and 1 is on v2)
		*
		* |-----| d
		* v1 ---x----- v2
		*
		*/
		static float Interpolate(float v1, float v2, float d);
	};
}