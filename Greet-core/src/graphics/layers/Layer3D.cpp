#include "Layer3D.h"

namespace Greet {

	Layer3D::Layer3D(Renderer3D* renderer)
		: m_renderer(renderer)
	{
	}

	Layer3D::~Layer3D()
	{
		delete m_renderer;
	}

	void Layer3D::Add(const EntityModel* renderable)
	{
		m_renderer->Submit(renderable);
	}

	void Layer3D::Render() const
	{
		SetUniforms();
		m_renderer->Begin();
		m_renderer->Render();
		m_renderer->End();
	}

	void Layer3D::Update(float timeElapsed)
	{
		m_renderer->Update(timeElapsed);
	}
}
