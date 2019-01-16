#include "GLayer.h"

#include <graphics/gui/GUIUtils.h>
#include <event/EventDispatcher.h>

namespace Greet {

  std::vector<Frame*> GLayer::frames;
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
      Vec2 pos = event.GetPosition() - (*it)->GetMargin().LeftTop();
      if((*it)->IsMouseInside(pos))
      {
        (*it)->OnMousePressed(event, pos - (*it)->GetPosition());
        return true;
      }
    }
    RequestFocus(nullptr);
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
      (*it)->OnMouseMoved(event, event.GetPosition() - (*it)->GetPosition() - (*it)->GetMargin().LeftTop());
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
      (*it)->OnWindowResize(width, height);
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
      (*it)->PreRender(renderer, Vec2(0,0));
      (*it)->RenderHandle(renderer);
      (*it)->PostRender(renderer);
    }

    renderer->End();
    renderer->Draw();
    shader.Disable();
  }

  void GLayer::Update(float timeElapsed)
  {
    for (auto it = frames.begin(); it != frames.end(); ++it)
    {
      (*it)->UpdateHandle(timeElapsed);
    }
  }

  // Could do checks if a popup is refusing to give request or something
  bool GLayer::RequestFocus(Component* component)
  {
    // Unfocus the currently focused component
    if(GetInstance()->m_focused)
      GetInstance()->m_focused->OnUnfocused();

    // Focus the requested one
    if(component && component != GetInstance()->m_focused)
      component->OnFocused();

    // TODO: Maybe clean this code up.
    Component* unfocused = GetInstance()->m_focused;
    Component* focused = component;;

    uint unfocusedDepth = unfocused ? unfocused->GetComponentDepth() : 0;
    uint focusedDepth = focused ? focused->GetComponentDepth() : 0;

    while(unfocusedDepth != focusedDepth)
    {
      if(unfocusedDepth > focusedDepth)
      {
        unfocused->ChildChangedFocus(false);
        unfocused = unfocused->GetParent();
        --unfocusedDepth;
      }
      else
      {
        focused->ChildChangedFocus(true);
        focused = focused->GetParent();
        --focusedDepth;
      }
    }

    while(unfocused != focused)
    {
      unfocused->ChildChangedFocus(false);
      unfocused = unfocused->GetParent();
      focused->ChildChangedFocus(true);
      focused = focused->GetParent();
    }

    GetInstance()->m_focused = component;

    return true;
  }

  void GLayer::AddFrame(Frame* frame)
  {
    if (frame == nullptr)
    {
      Log::Warning("Cannot add nullptr to frames");

      return;
    }
    frame->Measure();
    frame->MeasureFill(GetWidth(),GetHeight(), 1, true);
    frames.push_back(frame);
    frame->PostConstruction();
  }

  Frame* GLayer::RemoveFrame(const std::string& name)
  {
    for(auto it = frames.begin(); it!=frames.end();++it)
    {
      if((*it)->GetName() == name)
      {
        frames.erase(it);
        return (*it);
      }
    }
    return nullptr;
  }

  Frame* GLayer::RemoveFrame(Frame* frame)
  {
    for (auto it = frames.begin(); it != frames.end();++it)
    {
      if ((*it) == frame)
      {
        frames.erase(it);
        return frame;
      }
    }
    return nullptr;
  }

  Frame* GLayer::GetFrame(const std::string& name)
  {
    for(auto it = frames.begin(); it!=frames.end();++it)
    {
      if((*it)->GetName() == name)
      {
        return (*it);
      }
    }
    return nullptr;
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
