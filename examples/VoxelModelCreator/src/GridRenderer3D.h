#pragma once

#include <Greet.h>
#include "Cube.h"

namespace vmc
{

	class GridRenderer3D : public Greet::Renderer3D
	{
	private:
		Greet::Shader* simpleShader;

		Greet::EntityModel* emodel;
		Greet::Material* material;
		Greet::Mesh* mesh;
		Greet::MeshData* meshdata;


		Greet::Shader lineShader;
    Greet::VertexArray vao;
    Greet::Buffer vbo;
    Greet::Buffer ibo;
		uint* m_indices;
	public:

		GridRenderer3D();

		void UpdateShader();
		void Begin(Greet::Camera* camera) override;
		void DrawCube(Greet::Camera* camera, const Greet::Vec3& pos, const Greet::Vec3& size, uint color, bool culling);
		virtual void Submit(Greet::Camera* camera, const Cube& cube);
		void DrawLineCube(Greet::Camera* camera, const Greet::Vec3& pos, const Greet::Vec3& size, const Greet::Vec4& color);
		void DrawLine(Greet::Camera* camera, const Greet::Vec3& start, const Greet::Vec3& end, const Greet::Vec4& color);
	};
}
