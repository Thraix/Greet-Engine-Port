#include "Skybox.h"

#include <graphics/models/MeshFactory.h>
#include <graphics/shaders/ShaderFactory.h>


namespace Greet {
	
	Skybox::Skybox(const CubeMap& cubemap) : m_map(cubemap), m_shader(ShaderFactory::SkyboxShader())
	{
		MeshData* data = MeshFactory::Cube(0,0,0, 1,1,1);
		m_mesh = new Mesh(data);
		m_mesh->SetClockwiseRender(true);
		delete data;
	}

	Skybox::~Skybox()
	{
		delete m_mesh;
	}

	void Skybox::Render(const Mat4& projectionMatrix, const Camera& camera) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glDisable(GL_DEPTH_TEST));
		GLCall(glFrontFace(GL_CCW));
		m_shader.Enable();
		m_shader.SetUniformMat4("projectionMatrix", projectionMatrix);
		m_shader.SetUniformMat4("viewMatrix", Mat4::ViewMatrix(Vec3(0, 0, 0), camera.GetRotationVector()));
		m_map.Enable();
		m_mesh->Bind();
		m_mesh->Render();
		m_mesh->Unbind();
		m_map.Disable();
		m_shader.Disable();
		GLCall(glEnable(GL_DEPTH_TEST));
	}
}
