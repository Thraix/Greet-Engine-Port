#include "BatchRenderer3D.h"

namespace Greet {

	void BatchRenderer3D::Submit(const EntityModel* model)
	{
		for (BatchRenderer3DMap* map : m_map)
		{
			if (map->m_material == model->GetMaterialModel())
			{
				map->AddEntity(model);
				return;
			}
		}
		BatchRenderer3DMap* map = new BatchRenderer3DMap(model->GetMaterialModel());
		map->AddEntity(model);
		m_map.push_back(map);
	}

	void BatchRenderer3D::Render() const
	{
		GLCall(glDepthRange(m_near, m_far));
		const Mat4& viewMatrix = m_camera->GetViewMatrix();
		for (BatchRenderer3DMap* map : m_map)
		{
			map->m_material.GetMaterial().Bind();
			map->m_material.GetMaterial().GetShader().SetUniformMat4("projectionMatrix", m_projectionMatrix);
			map->m_material.GetMaterial().GetShader().SetUniformMat4("viewMatrix", viewMatrix);
			const Mesh& mesh = map->m_material.GetMesh();
			mesh.Bind();
			for (const EntityModel* model : map->m_models)
			{
				map->m_material.GetMaterial().GetShader().SetUniformMat4("transformationMatrix", model->GetTransformationMatrix());
				mesh.Render();
			}
			mesh.Unbind();
			map->m_material.GetMaterial().Unbind();
		}

	}
}