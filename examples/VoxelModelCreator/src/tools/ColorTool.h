#pragma once

#include "Tool.h"

namespace vmc {
	class ColorTool : public Tool
	{
	public:
		ColorTool(Grid* grid) : Tool(grid) {}

		bool OnRightClick() override;
		bool OnLeftClick() override;
	};
}