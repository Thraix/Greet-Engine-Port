#include "RenderEngine.h"

namespace Greet {

	std::map<std::string, Layer*> RenderEngine::m_renderer2ds;

	std::map<std::string, Layer3D*> RenderEngine::m_renderer3ds;

	void RenderEngine::AddLayer2d(Layer* renderer, const std::string& name)
	{
		if (renderer == NULL)
		{
			Log::Error("Trying to add Renderer2D to RenderEngine but it is NULL.");
			return;
		}
		m_renderer2ds.emplace(name, renderer);
	}

	void RenderEngine::AddLayer3d(Layer3D* renderer, const std::string& name)
	{
		if (renderer == NULL)
		{
			Log::Error("Trying to add Renderer3D to RenderEngine but it is NULL.");
			return;
		}
		m_renderer3ds.emplace(name, renderer);
	}

	Layer* RenderEngine::RemoveLayer2d(const std::string& name)
	{
		auto it = m_renderer2ds.find(name);
		m_renderer2ds.erase(it);
		return it->second;
	}

	Layer3D* RenderEngine::RemoveLayer3d(const std::string& name)
	{
		auto it = m_renderer3ds.find(name);
		m_renderer3ds.erase(it);
		return it->second;
	}

	Layer* RenderEngine::GetRenderer2d(const std::string& name)
	{
		auto it = m_renderer2ds.find(name);
		return it->second;
	}

	Layer3D* RenderEngine::GetRenderer3d(const std::string& name)
	{
		auto it = m_renderer3ds.find(name);
		return it->second;
	}

	void RenderEngine::Render()
	{
		for (auto it = m_renderer3ds.begin(); it != m_renderer3ds.end(); it++)
		{
      it->second->PreRender();
			it->second->Render();
      it->second->PostRender();
		}

		for (auto it = m_renderer2ds.begin(); it != m_renderer2ds.end(); it++)
		{
			it->second->Render();
		}
	}

	void RenderEngine::Update(float timeElapsed)
	{
		for (auto it = m_renderer3ds.begin(); it != m_renderer3ds.end(); it++)
		{
			it->second->Update(timeElapsed);
		}

		for (auto it = m_renderer2ds.begin(); it != m_renderer2ds.end(); it++)
		{
			it->second->Update(timeElapsed);
		}
	}

}
