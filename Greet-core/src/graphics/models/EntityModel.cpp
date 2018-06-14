#include "EntityModel.h"

#include <graphics/renderers/Renderer3D.h>

namespace Greet {
	EntityModel::EntityModel(const MaterialModel* model, float x, float y, float z, float sx, float sy, float sz, float rx, float ry, float rz)
		: m_model(model), m_position(Vec3(x,y,z)), m_scale(Vec3(sx,sy,sz)), m_rotation(Vec3(rx,ry,rz))
	{
		UpdateTransformation();
	}

	EntityModel::EntityModel(const MaterialModel* model, Vec3 position, Vec3 scale, Vec3 rotation)
		: m_model(model), m_position(position), m_scale(scale), m_rotation(rotation)
	{
		UpdateTransformation();
	}

	EntityModel::EntityModel(const MaterialModel* model)
    :m_model(model), m_position(Vec3(0,0,0)), m_scale(Vec3(1,1,1)), m_rotation(0,0,0)
  {
		UpdateTransformation();
  }

  void EntityModel::PreRender(const Renderer3D* renderer) const
  {
    m_model->PreRender(renderer,m_transformationMatrix);
  }

  void EntityModel::Render(const Renderer3D* renderer) const
  {
    m_model->Render(renderer,m_transformationMatrix);
  }

  void EntityModel::PostRender(const Renderer3D* renderer) const
  {
    m_model->PostRender(renderer,m_transformationMatrix);
  }

	void EntityModel::Update(float timeElapsed)
	{
		if (m_hasChanged)
		{
			UpdateTransformation();
		}
	}

	void EntityModel::UpdateTransformation()
	{
		m_transformationMatrix = Mat4::TransformationMatrix(m_position, m_rotation, m_scale);
		m_hasChanged = false;
	}
}
