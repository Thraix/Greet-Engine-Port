#pragma once
#include <Greet.h>
#include "Cube.h"

namespace vmc
{

	class GridRenderer3D : public Greet::Renderer3D
	{
	private:
    Greet::Ref<Greet::Shader> simpleShader;

		Greet::EntityModel* emodel;
		Greet::Material* material;
		Greet::Mesh* mesh;


    Greet::Ref<Greet::Shader> lineShader;
    Greet::Ref<Greet::VertexArray> vao;
    Greet::Ref<Greet::VertexBuffer> vbo;
    Greet::Ref<Greet::Buffer> ibo;
		uint* m_indices;
	public:

		GridRenderer3D();

		void Begin(Greet::Camera* camera) override;
		void DrawCube(Greet::Camera* camera, const Greet::Vec3<float>& pos, const Greet::Vec3<float>& size, uint color, bool culling);
		virtual void Submit(Greet::Camera* camera, const Cube& cube);
		void DrawLineCube(Greet::Camera* camera, const Greet::Vec3<float>& pos, const Greet::Vec3<float>& size, const Greet::Vec4& color);
		void DrawLine(Greet::Camera* camera, const Greet::Vec3<float>& start, const Greet::Vec3<float>& end, const Greet::Vec4& color);
	};
}
