#include "graphics/models/MaterialModel.h"

namespace Greet {

	uint MaterialModel::s_current_id;

	MaterialModel::MaterialModel(const Mesh* mesh, const Material* material)
		: m_mesh(mesh), m_material(material), m_id(s_current_id++)
	{
		
	}

	MaterialModel::~MaterialModel()
	{
	}


	bool MaterialModel::operator==(const MaterialModel& second) const
	{
		return m_id == second.m_id;
	}
}
