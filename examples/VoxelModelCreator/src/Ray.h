#pragma once

#include <Greet.h>
#include <vector>
#include "Cube.h"

namespace vmc { namespace Ray
{
	std::vector<Cube> GetCubeRay(Greet::Vec3 near, Greet::Vec3 direction, uint gridSize);
	bool RayInside(float plane1, float plane2, float min1, float max1, float min2, float max2);
	float GetIntersectionT(float planePos, float start, float invDirection);

	
}}
