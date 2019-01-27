#include "ModelExport.h"

#include "Grid.h"

namespace vmc
{
	using namespace Greet;

	std::deque<Square> ModelExport::GetAllSquares(const std::set<Cube, CubeCompare>& cubes)
	{
		std::deque<Square> squares;
 		bool* grid = new bool[Grid::GRID_SIZE*Grid::GRID_SIZE];
		uint minX, maxX, minY, maxY, minZ, maxZ;

		CalculateBoundingBox(cubes, &minX,&maxX, &minY,&maxY,&minZ,&maxZ);
		GenXMaps(minX,maxX, minY, maxY, minZ, maxZ, cubes, &squares);

		return squares;
	}

	void ModelExport::GenXMaps(uint minX, uint maxX, uint minY, uint maxY, uint minZ, uint maxZ, const std::set<Cube, CubeCompare>& cubes, std::deque<Square>* squares)
	{

		std::unordered_map<uint, bool*> colorMapsPos;
		std::unordered_map<uint, bool*> colorMapsNeg;
		std::deque<bool*> buffers;

		uint diffY = maxY - minY;
		uint diffZ = maxZ - minZ;
		// Current layer
		for (uint x = minX; x < maxX; ++x)
		{
			// Go through each cube in layer
			for (uint y = minY; y < maxY; ++y)
			{
				for (uint z = minZ; z < maxZ; ++z)
				{
					auto cube = cubes.find(Cube::Hash(x, y, z));

					if (cube != cubes.end())
					{
						// Determain if the cube side will be visible
						if (!Cube::ValidPos(x - 1) || cubes.count(Cube::Hash(x - 1, y, z)) == 0)
						{
							AddCubeSideToMap(cube->y, cube->z, cube->color, minY, minZ, diffY, diffZ, &colorMapsNeg, &buffers);
						}

						// Determain if the cube side will be visible
						if (!Cube::ValidPos(x + 1) || cubes.count(Cube::Hash(x + 1, y, z)) == 0)
						{
							AddCubeSideToMap(cube->y, cube->z, cube->color, minY, minZ, diffY, diffZ, &colorMapsPos, &buffers);
						}
					}
				}
			}

			// Calculate the squares.


			ResetMaps(&colorMapsPos, &buffers);
			ResetMaps(&colorMapsNeg, &buffers);
		}
	}

	void ModelExport::FindAllSquares(bool* pixels, uint width, uint height, std::deque<Square>* squares)
	{
		uint* heightBuffer = new uint[width];
		uint pixelCount = 0;

		while (pixelCount > 0)
		{
			Square square = FindBiggestSquare(pixels,width,height, heightBuffer);
			pixelCount -= (square.p2 - square.p2).x * (square.p2 - square.p2).y; // Area of the square
			squares->push_back(square);
		}

		delete heightBuffer;
	}

	Square ModelExport::FindBiggestSquare(bool* buffer, uint width, uint height, uint* histogram)
	{
		// Set the height of the "graphs"
		for (int x = 0; x < width; x++)
		{
			uint y = 0;
			while (buffer[x+y*width] && y <= height)
			{
				histogram[x]++;
				y++;
			}
		}
		for (int y = 0;y < height;y++)
		{
			for (int x = 0; x < width; x++)
			{
				uint area = histogram[x];
				for (int x2 = x + 1;x2 < width;x2++)
				{
					
				}
			}
		}
		return Square();
	}


	void ModelExport::AddCubeSideToMap(uint p1, uint p2, uint color, uint min1, uint min2, uint diff1, uint diff2, std::unordered_map<uint, bool*>* colorMaps, std::deque<bool*>* buffers)
	{
		uint y = p1 - min1;
		uint z = p2 - min2;
		// Check if we already have a map with a certain color
		auto colorMap = colorMaps->find(color);
		if (colorMap != colorMaps->end())
		{
			colorMap->second[y + z * diff1] = true;
		}
		else // Otherwise add the buffer to the colorMap
		{
			// Check if we need to create a buffer.
			if (buffers->size() == 0)
			{
				bool* buffer = GenBuffer(diff1, diff2);

				// Clear memory in case of clutter
				memset(buffer, 0, diff1 * diff2);
				buffer[y + z * diff1] = true;
				colorMaps->emplace(color, buffer);
			}
			else // Otherwise use an existing one.
			{
				// Get a stored buffer
				bool* buffer = *buffers->begin();
				memset(buffer, 0, diff1 * diff2);
				buffer[y + z * diff1] = true;
				colorMaps->emplace(color, buffer);

				// remove from available buffers
				buffers->pop_back();
			}
		}
	}

	void ModelExport::CalculateSquares(bool* map, uint diff1, uint diff2, std::deque<Square>* squares)
	{
		
	}

	// Generate a buffer with size width*height
	bool* ModelExport::GenBuffer(uint width, uint height)
	{
		return new bool[width*height];
	}

	// Delete a generated buffer.
	void ModelExport::DeleteBuffer(bool* buffer)
	{
		if(buffer != NULL)
			delete[] buffer;
	}

	// Calculate the minimum bounding box that can fit the model.
	void ModelExport::CalculateBoundingBox(const std::set<Cube, CubeCompare>& cubes, uint* minX, uint* maxX, uint* minY, uint* maxY, uint* minZ, uint* maxZ)
	{
		auto it = cubes.begin();

		// Initialize the variables
		*minX = it->x;
		*maxX = it->x;
		*minY = it->y;
		*maxY = it->y;
		*minZ = it->z;
		*maxZ = it->z;

		++it;

		for (; it != cubes.end(); ++it)
		{
			if (it->x < *minX)
				*minX = it->x;
			else if (it->x > *maxX) // variable cannot be min and max at the same time.
				*maxX = it->x;

			if (it->y < *minY)
				*minY = it->y;
			else if (it->y > *maxY) // variable cannot be min and max at the same time.
				*maxY = it->y;

			if (it->z < *minZ)
				*minX = it->z;
			else if (it->z > *maxZ) // variable cannot be min and max at the same time.
				*maxZ = it->z;
		}
	}

	void ModelExport::ResetMaps(std::unordered_map<uint, bool*>* colorMaps, std::deque<bool*>* buffers)
	{
		// Move all color maps to the deque.
		auto mapIt = colorMaps->begin();
		while (mapIt != colorMaps->end())
		{
			auto it = mapIt;
			++mapIt;

			buffers->push_back(it->second);
			colorMaps->erase(it);
		}
	}

	void ModelExport::SaveImageLayers(const std::unordered_map<uint, bool*>& colorMaps, uint diff1, uint diff2, int p, int dir)
	{
		// Now we have 2d maps of all the color layers in colorMaps.
		for (auto mapIt = colorMaps.begin(); mapIt != colorMaps.end(); ++mapIt)
		{
			BYTE* bytes = new BYTE[3 * diff1 * diff2];
			for (int i = 0;i < diff1*diff2;i++)
			{
				if (mapIt->second[i])
				{
					bytes[i * 3] = (mapIt->first >> 0) & 0xff;
					bytes[i * 3 + 1] = (mapIt->first >> 16) & 0xff;
					bytes[i * 3 + 2] = (mapIt->first >> 24) & 0xff;
				}
				else
				{
					bytes[i * 3] = 0;
					bytes[i * 3 + 1] = 0;
					bytes[i * 3 + 2] = 0;
				}
			}
			std::string s = "modelImages/layer"+std::to_string(dir)+"_" + std::to_string(p) + "_color_" + std::to_string(mapIt->first) + ".png";
			FIBITMAP* Image = FreeImage_ConvertFromRawBits(bytes, diff1, diff2, 3 * diff1, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
			FreeImage_Save(FIF_PNG, Image, s.c_str(), 0);
			delete[] bytes;
		}
	}
}