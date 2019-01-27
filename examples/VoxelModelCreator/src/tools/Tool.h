#pragma once

#include "../Cube.h"

namespace vmc
{
	class Grid;

	class Tool
	{
	protected:
		Grid* m_grid;
	public:
		Tool(Grid* grid) : m_grid(grid) {}
		virtual bool OnRightClick() = 0;
		virtual bool OnLeftClick() = 0;
		virtual void Update(float timeElapsed) {};
		virtual void Render() {};
	};

}