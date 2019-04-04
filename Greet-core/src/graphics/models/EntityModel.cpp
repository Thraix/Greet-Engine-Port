#include "EntityModel.h"

#include <graphics/renderers/Renderer3D.h>

namespace Greet {
  EntityModel::EntityModel(const MaterialModel* model, float x, float y, float z, float sx, float sy, float sz, float rx, float ry, float rz)
    : m_model(model), m_position(Vec3<float>(x,y,z)), m_scale(Vec3<float>(sx,sy,sz)), m_rotation(Vec3<float>(rx,ry,rz))
  {
    UpdateTransformation();
  }

  EntityModel::EntityModel(const MaterialModel* model, Vec3<float> position, Vec3<float> scale, Vec3<float> rotation)
    : m_model(model), m_position(position), m_scale(scale), m_rotation(rotation)
  {
    UpdateTransformation();
  }

  EntityModel::EntityModel(const MaterialModel* model)
    :m_model(model), m_position(Vec3<float>(0,0,0)), m_scale(Vec3<float>(1,1,1)), m_rotation(0,0,0)
  {
    UpdateTransformation();
  }

  void EntityModel::PreRender(const Renderer3D* renderer, const Camera* camera) const
  {
    m_model->PreRender(renderer,camera, m_transformationMatrix);
  }

  void EntityModel::Render(const Renderer3D* renderer, const Camera* camera) const
  {
    m_model->Render(renderer,camera, m_transformationMatrix);
  }

  void EntityModel::PostRender(const Renderer3D* renderer, const Camera* camera) const
  {
    m_model->PostRender(renderer,camera, m_transformationMatrix);
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
