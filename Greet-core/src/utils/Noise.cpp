#include "Noise.h"
#define _USE_MATH_DEFINES
#include <string>
#include <cmath>
#include <logging/Log.h>

namespace Greet {
	
	float* Noise::GenNoise(uint width, uint height, uint octave, uint frequencyX, uint frequencyY, float persistance)
	{
		uint noiseWidth = (frequencyX << (octave-1)) + 3;
		uint noiseHeight = (frequencyY << (octave-1)) + 3;
		float* noise = new float[noiseWidth*noiseHeight];
		float* smoothNoise = new float[(noiseWidth-2)*(noiseWidth-2)];
		for (uint y = 0;y < noiseHeight;y++)
		{
			for (uint x = 0;x < noiseWidth;x++)
			{
				noise[x + y*noiseWidth] = PRNG(x,y);
			}
		}
		for (uint y = 1;y < noiseHeight-1;y++)
		{
			for (uint x = 1;x < noiseWidth-1;x++)
			{
				smoothNoise[x-1 + (y-1)*(noiseWidth-2)] = Smooth(x, y, noise, noiseWidth, noiseHeight);
			}
		}

		float* persistances = new float[octave];
		persistances[0] = persistance;
		float totalPersistance = persistance;
		for (uint i = 1;i < octave;i++)
		{
			persistances[i] = persistances[i - 1] * persistance;
			totalPersistance += persistances[i];
		}

		for (uint i = 1;i < octave;i++)
		{
			persistances[i] = persistances[i] / totalPersistance;
		}
		float* result = new float[width * height];

		for (uint y = 0;y < height;y++)
		{
			for (uint x = 0;x < width;x++)
			{
				float value = 0;
				for (uint i = 0;i < octave;i++)
				{
					float d1 = x * frequencyX*(i + 1) / (float)width;
					float d2 = y * frequencyY*(i + 1) / (float)height;
					uint noiseX = (uint)floor(d1);
					uint noiseY = (uint)floor(d2);
					value += Interpolate(smoothNoise[noiseX + noiseY*(noiseWidth-2)],smoothNoise[noiseX+1+noiseY*(noiseWidth-2)],smoothNoise[noiseX+(noiseY+1)*(noiseWidth-2)],smoothNoise[noiseX + 1 + (noiseY + 1) * (noiseWidth - 2)],d1-noiseX,d2-noiseY)*persistances[i];
				}
				result[x + y * width] = value;
			}
		}
		delete[] noise;
		delete[] smoothNoise;
		delete[] persistances;

		return result;
	}

	float Noise::Smooth(uint noiseX, uint noiseY, const float* noise, uint noiseWidth, uint noiseHeight)
	{
		float center = noise[noiseX + noiseY*noiseWidth]*0.5f;
		float edges = (noise[noiseX - 1 + noiseY * noiseWidth] + noise[noiseX + 1 + noiseY * noiseWidth] + noise[noiseX + (noiseY - 1) * noiseWidth] + noise[noiseX + (noiseY + 1) * noiseWidth])*0.08333f;
		float corners = (noise[noiseX-1 + (noiseY-1) * noiseWidth] + noise[noiseX + 1 + (noiseY-1) * noiseWidth]+noise[noiseX-1 + (noiseY+1) * noiseWidth]+noise[noiseX+1 + (noiseY+1) * noiseWidth])*0.0416666f;
		return center + edges + corners;
	}

	float Noise::PRNG(int x, int y)
	{
		int n = x + y * 141263;
		n = (n << 11) ^ n;
		return (float)( ( ( n * (n * n * 20947 + 794327) + 1376312589) & 0x7fffffff) / 1073741824.0*0.5);
	}
	
	float Noise::Interpolate(float v1, float v2, float v3, float v4, float d1, float d2)
	{
		return Interpolate(Interpolate(v1, v2, d1), Interpolate(v3, v4, d1), d2);
	}

	float Noise::Interpolate(float v1, float v2, float d)
	{
		float c = 0.5f+cos(d * M_PI) * 0.5f;
		float v = (v1 - v2)*c + v2;

		return v;
	}
}
