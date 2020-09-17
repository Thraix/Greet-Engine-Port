#include "ColorTool.h"

#include "../Grid.h"

namespace vmc {
	using namespace Greet;
	bool ColorTool::OnRightClick() {
		if (m_grid->HasSelected())
		{
			Color color = Color{m_grid->GetSelectedCube().color};
			//m_grid->GetColorPicker()->SetColorAsRGB(color);
			return true;
		}
		return false;
	};

	bool ColorTool::OnLeftClick() {
		if (m_grid->HasSelected())
		{
			Color color = Color{m_grid->GetSelectedCube().color};
			//m_grid->GetColorPicker()->SetColorAsRGB(color);
			return true;
		}
		return false;
	};
}
