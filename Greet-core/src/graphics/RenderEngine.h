#pragma once

#include <map>
#include <graphics/layers/Layer.h>
#include <graphics/layers/Layer3D.h>
#include <graphics/layers/GUILayer.h>

namespace Greet {
	class RenderEngine
	{

	private:
		static std::map<std::string, Layer*> m_renderer2ds;
		static std::map<std::string, Layer3D*> m_renderer3ds;

	public:
		static void AddLayer2d(Layer* renderer, const std::string& name);
		static void AddLayer3d(Layer3D* renderer, const std::string& name);

		static Layer* RemoveLayer2d(const std::string& name);
		static Layer3D* RemoveLayer3d(const std::string& name);
		static Layer* GetRenderer2d(const std::string& name);
		static Layer3D* GetRenderer3d(const std::string& name);
		static void Render();
		static void Update(float timeElapsed);


		
	};
}