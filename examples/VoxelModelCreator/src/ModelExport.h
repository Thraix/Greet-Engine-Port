#pragma once

#include <math/Vec4.h>
#include <utility>
#include <set>
#include <unordered_map>

#include "Cube.h"
namespace vmc
{

	struct Square
	{
		Greet::Vec3<float> p1;
		Greet::Vec3<float> p2;
		uint color;
	};

	class ModelExport {
	private:

	public:
		static std::deque<Square> GetAllSquares(const std::set<Cube, CubeCompare>& cubes);

	private:

		static void GenXMaps(uint minX, uint maxX, uint minY, uint maxY, uint minZ, uint maxZ, const std::set<Cube, CubeCompare>& cubes, std::deque<Square>* squares);
		static void AddCubeSideToMap(uint p1, uint p2, uint color, uint min1, uint min2, uint diff1, uint diff2, std::unordered_map<uint, bool*>* colorMaps, std::deque<bool*>* buffers);
		static void CalculateSquares(bool* map, uint diff1, uint diff2, std::deque<Square>* squares);
		static void FindAllSquares(bool* pixels, uint width, uint height, std::deque<Square>* squares);
		static Square FindBiggestSquare(bool* buffer, uint width, uint height, uint* histogram);
		static bool* GenBuffer(uint width, uint height);
		static void DeleteBuffer(bool* buffer);
		static void CalculateBoundingBox(const std::set<Cube, CubeCompare>& m_grid, uint* minX, uint* maxX, uint* minY, uint* maxY, uint* minZ, uint* maxZ);
		static void ResetMaps(std::unordered_map<uint, bool*>* colorMaps, std::deque<bool*>* buffers);
		static void SaveImageLayers(const std::unordered_map<uint, bool*>& colorMaps, uint diff1, uint diff2, int p, int dir);
	};

}
