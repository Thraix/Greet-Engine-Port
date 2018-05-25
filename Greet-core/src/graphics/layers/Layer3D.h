#pragma once

#include <graphics/shaders/Shader.h>
#include <graphics/Renderable.h>
#include <graphics/renderers/Renderer3D.h>
#include <logging/Log.h>

namespace Greet {
	class Layer3D
	{
	protected:
		Renderer3D* m_renderer;
	public:
		Layer3D(Renderer3D* renderer);
		virtual ~Layer3D();
		void Add(const EntityModel* renderable);
		virtual void Render() const;
		virtual void Update(float timeElapsed);
		virtual void ViewportResize(float x, float y, float width, float height) {}
		virtual void WindowResize(int width, int height) {}

		
	protected:
		virtual void SetUniforms() const {}
	};
}
