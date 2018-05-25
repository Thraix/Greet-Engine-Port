#include "MeshData.h"
#include <logging/Log.h>

namespace Greet {

	MeshData::MeshData(Vec3* vertices, uint vertexCount, uint* indices, uint indexCount)
		: m_vertices(vertices), m_indices(indices), m_vertexCount(vertexCount), m_indexCount(indexCount)
	{
		
	}

	MeshData::~MeshData()
	{
		for (uint i = 0;i < m_data.size();i++)
		{
			delete m_data[i];
		}
		delete m_vertices;
		delete m_indices;
	}

	void MeshData::AddAttribute(AttributeData* data)
	{
		m_data.push_back(data);
	}

	AttributeData* MeshData::GetAttribute(AttributeDefaults defaults) const
	{
		for (auto it = m_data.begin(); it != m_data.end(); it++)
		{
			if ((*it)->location == defaults.location)
			{
				return *it;
			}
		}
		return NULL;
	}
}