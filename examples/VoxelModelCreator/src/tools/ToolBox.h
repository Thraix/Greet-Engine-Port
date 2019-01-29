#pragma once

#include "Tool.h"
#include "PlaceTool.h"
#include "ColorTool.h"
#include <vector>

namespace vmc
{
	class ToolBox
	{
		std::vector<Tool*> m_tools;
		std::vector<Tool*>::iterator m_selectedTool;
	public:
		ToolBox(Grid* grid)
		{
			m_tools.push_back(new PlaceTool(grid));
			m_tools.push_back(new ColorTool(grid));
			m_selectedTool = m_tools.begin();
		}

		virtual ~ToolBox()
		{
			// Clear tool memory
			for (auto it = m_tools.begin();it != m_tools.end();++it)
			{
				delete *it;
			}
		}

		bool OnRightClick()
		{
			return (*m_selectedTool)->OnRightClick();
		};

		bool OnLeftClick() 
		{
			return (*m_selectedTool)->OnLeftClick();
		};

		void NextTool()
		{
			m_selectedTool++;
			if (m_selectedTool == m_tools.end())
				m_selectedTool = m_tools.begin();
		}

		void PreviousTool()
		{
			if (m_selectedTool == m_tools.begin())
				m_selectedTool = m_tools.end();
			m_selectedTool--;
		}

		void Update(float timeElapsed) 
		{
			(*m_selectedTool)->Update(timeElapsed);
		};

		void Render() 
		{
			(*m_selectedTool)->Render();
		};
	};
}