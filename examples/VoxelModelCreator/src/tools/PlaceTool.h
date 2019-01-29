#pragma once

#include "Tool.h"

namespace vmc {
	class PlaceTool : public Tool
	{
	public:
		PlaceTool(Grid* grid) : Tool(grid) {}

		bool OnRightClick() override;
		bool OnLeftClick() override;
	};
}