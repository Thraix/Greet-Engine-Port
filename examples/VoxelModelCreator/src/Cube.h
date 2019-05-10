#pragma once

#include <Greet.h>

namespace vmc
{
	class Cube
	{
	public:
		uint x;
		uint y;
		uint z;
		uint color;
		static const uint MAX_POSITION; // 1000
	public:
		Cube(uint x, uint y, uint z, uint color)
			: x(x), y(y), z(z), color(color)
		{
			if (x >= MAX_POSITION)
				Greet::Log::Error("Cube x position out of bounds (0-999): ", x);
			if(y >= MAX_POSITION)
				Greet::Log::Error("Cube y position out of bounds (0-999): ", y);
			if(z >= MAX_POSITION)
				Greet::Log::Error("Cube z position out of bounds (0-999): ", z);
		}

		Cube(uint x, uint y, uint z)
			: Cube(x, y, z, 0xffffffff)
		{

		}

		Cube()
			: Cube(0, 0, 0, 0xffffffff)
		{

		}

    Greet::Vec3<float> GetPosition() const
		{
			return Greet::Vec3<float>(x, y, z);
		}

		// Only Allowes 1000 x 1000 x 1000 grids.
		static uint Hash(uint x, uint y, uint z)
		{
			return x + y * MAX_POSITION + z * MAX_POSITION * MAX_POSITION;
		}

		static uint Hash(const Cube& cube)
		{
			return Hash(cube.x,cube.y,cube.z);
		}

		static bool ValidCube(int x, int y, int z)
		{
			return ValidPos(x) && ValidPos(y) && ValidPos(z);
		}

		inline static bool ValidPos(int i)
		{
			return i >= 0 && i < MAX_POSITION;
		}

		friend bool operator<(const Cube& c1, const Cube& c2)
		{
			return Hash(c1) < Hash(c2);
		}

		friend bool operator<(uint hash, const Cube& c)
		{
			return hash < Hash(c);
		}

		friend bool operator<(const Cube& c, uint hash)
		{
			return Hash(c) < hash;
		}

		friend bool operator==(const Cube& c1, const Cube& c2)
		{
			return Hash(c1) == Hash(c2);
		}

		friend bool operator!=(const Cube& c1, const Cube& c2)
		{
			return Hash(c1) != Hash(c2);
		}
	};

	struct CubeCompare
	{
		using is_transparent = void;

		bool operator()(const Cube& c1, const Cube& c2) const
		{
			return c1 < c2;
		}

		bool operator()(uint i, const Cube& c) const
		{
			return i < c;
		}

		bool operator()(const Cube& c, uint i) const
		{
			return c < i;
		}
	};
}
