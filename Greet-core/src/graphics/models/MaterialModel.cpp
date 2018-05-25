#include "graphics/models/MaterialModel.h"

namespace Greet {

	uint MaterialModel::s_current_id;

	MaterialModel::MaterialModel(Mesh* mesh, const Material& material)
		: m_mesh(mesh), m_material(material), m_id(s_current_id++)
	{
		
	}

	MaterialModel::~MaterialModel()
	{
		delete m_mesh;
	}


	bool MaterialModel::operator==(const MaterialModel& second) const
	{
		return m_id == second.m_id;
	}
}