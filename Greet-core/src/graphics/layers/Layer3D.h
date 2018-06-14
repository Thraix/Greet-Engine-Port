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
    virtual void PreRender() const;
		virtual void Render() const;
    virtual void PostRender() const;
		virtual void Update(float timeElapsed);
	};
}
