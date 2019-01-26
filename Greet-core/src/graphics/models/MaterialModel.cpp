#include "MaterialModel.h"

#include <graphics/renderers/Renderer3D.h>

namespace Greet {

  uint MaterialModel::s_current_id;

  MaterialModel::MaterialModel(const Mesh* mesh, const Material* material)
    : m_mesh(mesh), m_material(material), m_id(s_current_id++)
  {

  }

  MaterialModel::~MaterialModel()
  {
  }

  void MaterialModel::PreRender(const Renderer3D* renderer, const Camera* camera, const Mat4& transformationMatrix) const
  {
    GetMaterial().Bind();
    GetMaterial().GetShader().SetUniformMat4("transformationMatrix", transformationMatrix);
    GetMaterial().GetShader().SetUniformMat4("projectionMatrix", camera->GetProjectionMatrix());
    GetMaterial().GetShader().SetUniformMat4("viewMatrix", camera->GetViewMatrix());
  }

  void MaterialModel::Render(const Renderer3D* renderer, const Camera* camera, const Mat4& transformationMatrix) const
  {
    m_mesh->Bind();
    m_mesh->Render();
    m_mesh->Unbind();
  }

  void MaterialModel::PostRender(const Renderer3D* renderer, const Camera* camera, const Mat4& transformationMatrix) const
  {
    GetMaterial().Unbind();
  }

  void MaterialModel::PreRender(const Renderer3D* renderer, const Camera* camera) const
  {
    GetMaterial().Bind();
    GetMaterial().GetShader().SetUniformMat4("projectionMatrix", camera->GetProjectionMatrix());
    GetMaterial().GetShader().SetUniformMat4("viewMatrix", camera->GetViewMatrix());
  }

  void MaterialModel::Render(const Renderer3D* renderer, const Camera* camera) const
  {
    m_mesh->Bind();
    m_mesh->Render();
    m_mesh->Unbind();
  }

  void MaterialModel::PostRender(const Renderer3D* renderer, const Camera* camera) const
  {
    GetMaterial().Unbind();
  }

  bool MaterialModel::operator==(const MaterialModel& second) const
  {
    return m_id == second.m_id;
  }
}
