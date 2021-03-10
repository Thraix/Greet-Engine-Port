#include "EntityModel.h"

#include <graphics/renderers/Renderer3D.h>

namespace Greet {
  EntityModel::EntityModel(const Ref<Mesh>& mesh, const Ref<Material>& material, float x, float y, float z, float sx, float sy, float sz, float rx, float ry, float rz)
    : mesh{mesh}, material(material), m_position(Vec3f(x,y,z)), m_scale(Vec3f(sx,sy,sz)), m_rotation(Vec3f(rx,ry,rz))
  {
    UpdateTransformation();
  }

  EntityModel::EntityModel(const Ref<Mesh>& mesh, const Ref<Material>& material, Vec3f position, Vec3f scale, Vec3f rotation)
    : mesh{mesh}, material(material), m_position(position), m_scale(scale), m_rotation(rotation)
  {
    UpdateTransformation();
  }

  EntityModel::EntityModel(const Ref<Mesh>& mesh, const Ref<Material>& material)
    : mesh{mesh}, material(material), m_position(Vec3f(0,0,0)), m_scale(Vec3f(1,1,1)), m_rotation(0,0,0)
  {
    UpdateTransformation();
  }

  void EntityModel::BindShader() const
  {
    material->Bind();
    BindTransformation();
  }

  void EntityModel::BindTransformation() const
  {
    material->GetShader()->SetUniformMat4("uTransformationMatrix", m_transformationMatrix);

  }
  void EntityModel::PreRender() const
  {
    mesh->Bind();
  }

  void EntityModel::Render() const
  {
    mesh->Render();
  }

  void EntityModel::PostRender() const
  {
    mesh->Unbind();
  }

  void EntityModel::UnbindShader() const
  {
    GetMaterial()->Unbind();
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
    m_transformationMatrix = Mat4::TransformationMatrix(m_position, m_scale, m_rotation);
    m_hasChanged = false;
  }
}
