#include "Ray.h"



namespace vmc { namespace Ray {

	using namespace Greet;

	std::vector<Cube> GetCubeRay(Vec3 near, Vec3 direction, uint gridSize)
	{
		Vec3 invDirection = Vec3(1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z);
		std::vector<Cube> cubes;

		int x = floor(near.x);
		int y = floor(near.y);
		int z = floor(near.z);

		// If the near position is outside the work area, recalc position.
		if (near.x < 0 || near.x > gridSize || near.y < 0 || near.y > gridSize || near.z < 0 || near.z > gridSize)
		{
			// These will not give any cubes since we are "aiming" outside the box.
			if (near.x < 0 && direction.x <= 0)
				return cubes;
			if (near.y < 0 && direction.y <= 0)
				return cubes;
			if (near.z < 0 && direction.z <= 0)
				return cubes;
			if (near.x > 100 && direction.x >= 0)
				return cubes;
			if (near.y > 100 && direction.y >= 0)
				return cubes;
			if (near.z > 100 && direction.z >= 0)
				return cubes;

			// Calculate the t values for the plane intersections.
			//
			float tX = GetIntersectionT(near.x < 0 ? 0 : gridSize, near.x, invDirection.x);
			float tY = GetIntersectionT(near.y < 0 ? 0 : gridSize, near.y, invDirection.y);
			float tZ = GetIntersectionT(near.z < 0 ? 0 : gridSize, near.z, invDirection.z);

			// If x is outside, check if the tX value will intersect with the work area.
			if ((near.x < 0 || near.x > 100) && RayInside(near.y + tX*direction.y, near.z + tX*direction.z, 0, gridSize, 0, gridSize))
			{
				near += direction * tX;
				near.x = round(near.x);
				x = direction.x < 0 ? gridSize : 0;
				y = floor(near.y);
				z = floor(near.z);
			}
			// If y is outside, check if the tY value will intersect with the work area.
			else if ((near.y < 0 || near.y > 100) && RayInside(near.x + tY*direction.x, near.z + tY*direction.z, 0, gridSize, 0, gridSize))
			{
				near += direction * tY;
				near.y = round(near.y);
				y = direction.y < 0 ? gridSize : 0;
				x = floor(near.x);
				z = floor(near.z);
			}
			// If z is outside, check if the tZ value will intersect with the work area.
			else if ((near.z < 0 || near.z > 100) && RayInside(near.x + tZ*direction.x, near.y + tZ*direction.y, 0, gridSize, 0, gridSize))
			{
				near += direction * tZ;
				near.z = round(near.z);
				z = direction.z < 0 ? gridSize : 0;
				x = floor(near.x);
				y = floor(near.y);
			}
			// If none intersects with the work area there are no cubes that are ray traced.
			else
			{
				return cubes;
			}

		}

		// near is now clipping the work area in some way.
		//Vec3 pos = near;

		int dX = direction.x < 0 ? -1 : 1;
		int dY = direction.y < 0 ? -1 : 1;
		int dZ = direction.z < 0 ? -1 : 1;

		float tDeltaX = GetIntersectionT(dX, 0, invDirection.x);
		float tDeltaY = GetIntersectionT(dY, 0, invDirection.y);
		float tDeltaZ = GetIntersectionT(dZ, 0, invDirection.z);

		float tX = GetIntersectionT(direction.x < 0 ? floor(near.x - 0.0001f) : ceil(near.x + 0.0001f), near.x, invDirection.x);
		float tY = GetIntersectionT(direction.y < 0 ? floor(near.y - 0.0001f) : ceil(near.y + 0.0001f), near.y, invDirection.y);
		float tZ = GetIntersectionT(direction.z < 0 ? floor(near.z - 0.0001f) : ceil(near.z + 0.0001f), near.z, invDirection.z);

		while (true)
		{
			cubes.push_back(Cube(x, y, z, 0xff000000 | (rand() % 255) << 16 | (rand() % 255) << 8 | (rand() % 255)));
			float minT = std::min(tX, std::min(tY, tZ));
			if (tX == minT)
			{
				x += dX;
				if (x < 0 || x >= gridSize)
					break;
				tX += tDeltaX;
			}
			else if (tY == minT)
			{
				y += dY;
				if (y < 0 || y >= gridSize)
					break;
				tY += tDeltaY;
			}
			else // if(tZ == minT)
			{
				z += dZ;
				if (z < 0 || z >= gridSize)
					break;
				tZ += tDeltaZ;
			}
		}

		return cubes;
	}

	bool RayInside(float plane1, float plane2, float min1, float max1, float min2, float max2)
	{
		return plane1 >= min1 && plane1 < max1 && plane2 >= min2 && plane2 < max2;
	}

	float GetIntersectionT(float planePos, float start, float invDirection)
	{
		return (planePos - start) * invDirection;
	}
}}