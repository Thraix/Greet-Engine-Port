#include "GLayer.h"

#include <graphics/gui/GUIUtils.h>
#include <event/EventDispatcher.h>

namespace Greet {

  std::map<std::string, Frame*> GLayer::frames;
  GLayer* GLayer::instance;

  GLayer::GLayer(GUIRenderer* renderer, Shader&& shader)
    : m_renderer(renderer), m_shader(std::move(shader))
  {
    m_focused = nullptr;
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
      Vec2 pos = event.GetPosition() - it->second->GetMargin().LeftTop();
      if(it->second->IsMouseInside(pos))
      {
        it->second->OnMousePressed(event, pos - it->second->GetPosition());
        return true;
      }
    }
    return false;
  }

  void GLayer::OnReleased(const MouseReleasedEvent& event)
  {
    if (m_focused != nullptr)
      m_focused->MouseReleased(event, event.GetPosition() - m_focused->GetRealPosition());
  }

  void GLayer::OnMoved(const MouseMovedEvent& event)
  {
    if (m_focused != nullptr)
    {
      m_focused->MouseMoved(event, event.GetPosition() - m_focused->GetRealPosition());
      if(m_focused->UsingMouse())
        return;
    }

    for (auto it = frames.rbegin(); it != frames.rend(); ++it)
    {
      it->second->OnMouseMoved(event, event.GetPosition() - it->second->GetPosition() - it->second->GetMargin().LeftTop());
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
  void GLayer::OnTyped(const KeyTypedEvent& event)
  {
    if (m_focused != nullptr)
      m_focused->KeyTyped(event);
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

  void GLayer::CreateInstance(GUIRenderer* renderer, Shader&& shader)
  {
    // Keep shader as an rvalue
    instance = new GLayer(renderer, std::move(shader));
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

  // Could do checks if a popup is refusing to give request or something
  bool GLayer::RequestFocus(Component* component)
  {
    // Unfocus the currently focused component
    if(GetInstance()->m_focused)
      GetInstance()->m_focused->OnUnfocused();

    // Focus the requested one
    if(component != GetInstance()->m_focused)
      component->OnFocused();
    
    GetInstance()->m_focused = component;

    return true;
  }

  void GLayer::AddFrame(Frame* frame, const std::string& name)
  {
    if (frame == nullptr)
    {
      Log::Warning("Cannot add nullptr to frames");

      return;
    }
    frame->Measure();
    frame->MeasureFill(GetWidth(),GetHeight(), 1, true);
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
  float GLayer::GetWidth()
  {
    return Window::GetWidth();
  }
  float GLayer::GetHeight()
  {
    return Window::GetHeight();
  }
}
