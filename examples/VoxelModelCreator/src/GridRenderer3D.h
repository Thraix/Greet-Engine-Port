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

		void Begin(const Greet::Ref<Greet::Camera3D>& camera) override;
		void DrawCube(const Greet::Ref<Greet::Camera3D>& camera, const Greet::Vec3f& pos, const Greet::Vec3f& size, uint color, bool culling);
		virtual void Submit(const Greet::Ref<Greet::Camera3D>& camera, const Cube& cube);
		void DrawLineCube(const Greet::Ref<Greet::Camera3D>& camera, const Greet::Vec3f& pos, const Greet::Vec3f& size, const Greet::Vec4f& color);
		void DrawLine(const Greet::Ref<Greet::Camera3D>& camera, const Greet::Vec3f& start, const Greet::Vec3f& end, const Greet::Vec4f& color);
	};
}
