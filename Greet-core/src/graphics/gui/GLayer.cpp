#include "GLayer.h"

#include "GUIUtils.h"
#include <event/EventDispatcher.h>

namespace Greet {

	std::map<std::string, Frame*> GLayer::frames;
	GLayer* GLayer::instance;

	GLayer::GLayer(GUIRenderer* renderer, const Shader& shader)
		: m_renderer(renderer), m_shader(shader)
	{
		m_focused = nullptr;
    m_hovered = nullptr;
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
		for (auto it = frames.rbegin(); it != frames.rend(); ++it)
		{
      Content* c = it->second->OnMousePressed(event, event.GetPosition() - it->second->GetPosition());
			if (c)
			{
				if (c != m_focused)
				{
					if (m_focused)
						m_focused->OnUnfocused();
					c->OnFocused();
					m_focused = c;
				}
				return true;
			}
		}

		if (m_focused)
		{
			m_focused->OnUnfocused();
			m_focused = nullptr;
		}
		return false;
	}

	void GLayer::OnReleased(const MouseReleasedEvent& event)
	{
		if (m_focused != nullptr)
			m_focused->MouseReleased(event, event.GetPosition() - m_focused->GetPosition());
	}

	void GLayer::OnMoved(const MouseMovedEvent& event)
	{
		if (m_focused != nullptr)
			m_focused->MouseMoved(event, event.GetPosition() - m_focused->GetPosition());
		for (auto it = frames.rbegin(); it != frames.rend(); ++it)
		{
      Content* c = it->second->OnMouseMoved(event, event.GetPosition() - it->second->GetPosition());
			if (c)
			{
				if (c != m_hovered)
				{
					if (m_hovered != nullptr)
						m_hovered->MouseExited();
					c->MouseEntered();
					m_hovered = c;
				}
				return;
			}
		}

		if (m_hovered != nullptr)
		{
			m_hovered->MouseExited();
			m_hovered = nullptr;
		}
	}

	void GLayer::OnPressed(const KeyPressedEvent& event)
	{
		if (m_focused != nullptr)
			m_focused->KeyPressed(event);
	}

	void GLayer::OnReleased(const KeyReleasedEvent& event)
	{
		if (m_focused != nullptr)
			m_focused->KeyReleased(event);
	}

	void GLayer::WindowResize(int width, int height)
	{
		m_shader.Enable();
		m_shader.SetUniformMat3("pr_matrix", Mat3::Orthographic(0, Window::GetWidth(), 0, Window::GetHeight()));
		m_shader.Disable();
		for (auto it = frames.begin(); it != frames.end(); ++it)
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
		for (auto it = frames.begin(); it != frames.end(); ++it)
		{
			it->second->PreRender(renderer, Vec2(0,0));
			it->second->RenderHandle(renderer);
			it->second->PostRender(renderer);
		}
		renderer->End();
		renderer->Draw();
		shader.Disable();
	}

	void GLayer::Update(float timeElapsed)
	{
		for (auto it = frames.begin(); it != frames.end(); ++it)
		{
			it->second->UpdateHandle(timeElapsed);
		}
	}

	void GLayer::AddFrame(Frame* frame, const std::string& name)
	{
		if (frame == nullptr)
		{
			Log::Warning("Cannot add nullptr to frames");
			return;
		}
		frames.emplace(name, frame);
	}

	Frame* GLayer::RemoveFrame(const std::string& name)
	{
		auto it = frames.find(name);
		frames.erase(it);
		return it->second;
	}

	Frame* GLayer::RemoveFrame(Frame* frame)
	{
		for (auto it = frames.begin(); it != frames.end();++it)
		{
			if (it->second == frame)
			{
				frames.erase(it);
				return frame;
			}
		}
		return nullptr;
	}

	Frame* GLayer::GetFrame(const std::string& name)
	{
		auto it = frames.find(name);
		if (it == frames.end())
			return nullptr;
		return it->second;
	}
}
