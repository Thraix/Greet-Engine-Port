#include "PlaceTool.h"

#include "../Grid.h"

namespace vmc {
	bool PlaceTool::OnRightClick() {
		if (m_grid->HasAdjacent())
		{
			m_grid->Add(m_grid->GetAdjacentCube(),true);
			return true;
		}
		return false;
	};

	bool PlaceTool::OnLeftClick() {
		if (m_grid->HasSelected())
		{
			m_grid->Remove(m_grid->GetSelectedCube());
			return true;
		}
		return false;
	};
}