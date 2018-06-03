#include "GLayer.h"

#include "GUIUtils.h"
#include <event/EventDispatcher.h>

namespace Greet {

	std::map<std::string, Container*> GLayer::containers;
	GLayer* GLayer::instance;

	GLayer::GLayer(GUIRenderer* renderer, const Shader& shader)
		: m_renderer(renderer), m_shader(shader)
	{
		m_focused = NULL;
		EventDispatcher::AddKeyListener(100, *this);
		EventDispatcher::AddMouseListener(100, *this);
		Window::AddResizeCallback(this);

		GLint texIDs[32];
		for (int i = 0; i < 32; i++)
		{
			texIDs[i] = i;
		}
		m_shader.Enable();
		m_shader.SetUniformMat3("pr_matrix", Mat3::Orthographic(0, Window::GetWidth(), 0, Window::GetHeight()));
		m_shader.SetUniform1iv("textures", 32, texIDs);
		m_shader.Disable();
	}

	bool GLayer::OnPressed(const MousePressedEvent& event)
	{
		for (auto it = containers.rbegin(); it != containers.rend(); ++it)
		{
			if (it->second->MousePressHandle(event, event.GetPosition() - it->second->GetPosition()))
			{
				if (it->second != m_focused)
				{
					if (m_focused != NULL)
						m_focused->OnUnfocused();
					it->second->OnFocused();
					m_focused = it->second;
				}
				return true;
			}
		}

		if (m_focused != NULL)
		{
			m_focused->OnUnfocused();
			m_focused = NULL;
		}
		return false;
	}

	void GLayer::OnReleased(const MouseReleasedEvent& event)
	{
		if (m_focused != NULL)
			m_focused->MouseReleaseHandle(event, event.GetPosition() - m_focused->GetPosition());
	}

	void GLayer::OnMoved(const MouseMovedEvent& event)
	{
		if (m_focused != NULL)
			m_focused->MouseMoveHandle(event, event.GetPosition() - m_focused->GetPosition());
	}

	void GLayer::OnPressed(const KeyPressedEvent& event)
	{
		if (m_focused != NULL)
			m_focused->KeyPressedHandle(event);
	}

	void GLayer::OnReleased(const KeyReleasedEvent& event)
	{
		if (m_focused != NULL)
			m_focused->KeyReleasedHandle(event);
	}

	void GLayer::WindowResize(int width, int height)
	{
		m_shader.Enable();
		m_shader.SetUniformMat3("pr_matrix", Mat3::Orthographic(0, Window::GetWidth(), 0, Window::GetHeight()));
		m_shader.Disable();
		for (auto it = containers.begin(); it != containers.end(); ++it)
		{
			it->second->OnWindowResize(width, height);
		}
	}

	void GLayer::CreateInstance(GUIRenderer* renderer, const Shader& shader)
	{
		instance = new GLayer(renderer, shader);
	}

	GLayer* GLayer::GetInstance()
	{
		return instance;
	}

	void GLayer::DestroyInstance()
	{
		delete instance;
	}

	void GLayer::Render()
	{
		GUIRenderer* renderer = GetInstance()->m_renderer;
		const Shader& shader = GetInstance()->m_shader;
		shader.Enable();
		renderer->Begin();
		for (auto it = containers.begin(); it != containers.end(); ++it)
		{
			it->second->PreRender(renderer);
			it->second->RenderHandle(renderer, it->second->GetPosition());
			it->second->PostRender(renderer);
		}
		renderer->End();
		renderer->Draw();
		shader.Disable();
	}

	void GLayer::Update(float timeElapsed)
	{
		for (auto it = containers.begin(); it != containers.end(); ++it)
		{
			it->second->UpdateHandle(timeElapsed);
		}
	}

	void GLayer::AddContainer(Container* container, const std::string& name)
	{
		if (container == NULL)
		{
			Log::Warning("Cannot add NULL to containers");
			return;
		}
		containers.emplace(name, container);
	}

	Container* GLayer::RemoveContainer(const std::string& name)
	{
		auto it = containers.find(name);
		containers.erase(it);
		return it->second;
	}

	Container* GLayer::RemoveContainer(Container* container)
	{
		for (auto it = containers.begin(); it != containers.end();++it)
		{
			if (it->second == container)
			{
				containers.erase(it);
				return container;
			}
		}
		return NULL;
	}

	Container* GLayer::GetContainer(const std::string& name)
	{
		auto it = containers.find(name);
		if (it == containers.end())
			return NULL;
		return it->second;
	}
}
