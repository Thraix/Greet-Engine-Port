#include "GUIScene.h"

#include <graphics/gui/GUIUtils.h>
#include <event/EventDispatcher.h>
#include <graphics/gui/Frame.h>
#include <graphics/RenderCommand.h>

namespace Greet {

  GUIScene::GUIScene(GUIRenderer* renderer, const Ref<Shader>& shader)
    : m_renderer(renderer), m_shader(shader), projectionMatrix(Mat3::OrthographicViewport())
  {
    m_focused = nullptr;

    GLint texIDs[32];
    for (int i = 0; i < 32; i++)
    {
      texIDs[i] = i;
    }
    m_shader->Enable();
    m_shader->SetUniformMat3("pr_matrix", projectionMatrix);
    m_shader->SetUniform1iv("textures", 32, texIDs);
    m_shader->Disable();
  }

  void GUIScene::OnEvent(Event& event)
  {
    Scene::OnEvent(event);
    if(EVENT_IS_CATEGORY(event, INPUT_EVENT))
    {
      if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
        OnMousePressEventHandler((MousePressEvent&)event);
      else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
        OnMouseMoveEventHandler((MouseMoveEvent&)event);
      else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
        OnMouseReleaseEventHandler((MouseReleaseEvent&)event);
      else if(m_focused)
      {
        m_focused->OnEventHandler(event, m_focused->GetPosition());
        event.AddFlag(EVENT_HANDLED);
      }
    }
  }

  void GUIScene::OnMousePressEventHandler(MousePressEvent& event)
  {
    Vec2 mousePos = ~projectionMatrix * event.GetPosition();
    MousePressEvent transformedEvent{mousePos, event.GetButton()};

    for (auto it = frames.rbegin(); it != frames.rend(); ++it)
    {
      if((*it)->IsMouseInside(transformedEvent.GetPosition() - (*it)->GetMargin().LeftTop() - (*it)->GetPosition()))
      {
        (*it)->OnEventHandler(transformedEvent, (*it)->GetPosition());
        frames.splice(frames.end(), frames, std::next(it).base());
        event.AddFlag(EVENT_HANDLED | transformedEvent.GetFlags());
        return;
      }
    }
    RequestFocus(nullptr);
    event.AddFlag(EVENT_UNFOCUSED);
  }

  void GUIScene::OnMouseMoveEventHandler(MouseMoveEvent& event)
  {
    Vec2 mousePos = ~projectionMatrix * event.GetPosition();
    Vec2 mouseDelta  = ~projectionMatrix * (event.GetPosition() + event.GetDeltaPosition()) - mousePos;
    MouseMoveEvent transformedEvent{mousePos, mouseDelta};

    if (m_focused && m_focused->UsingMouse())
    {
      m_focused->OnEventHandler(transformedEvent, m_focused->GetRealPosition());
      event.AddFlag(EVENT_HANDLED | transformedEvent.GetFlags());
      return;
    }

    for (auto it = frames.rbegin(); it != frames.rend(); ++it)
    {
      (*it)->OnEventHandler(transformedEvent, (*it)->GetPosition());
    }
  }

  void GUIScene::OnMouseReleaseEventHandler(MouseReleaseEvent& event)
  {
    Vec2 mousePos = ~projectionMatrix * event.GetPosition();
    MouseReleaseEvent transformedEvent{mousePos, event.GetButton()};
    if (m_focused)
    {
      m_focused->OnEventHandler(transformedEvent, m_focused->GetRealPosition());
      event.AddFlag(EVENT_HANDLED | transformedEvent.GetFlags());
    }
  }

  void GUIScene::ViewportResize(ViewportResizeEvent& event)
  {
    projectionMatrix = Mat3::OrthographicViewport();
    m_shader->Enable();
    m_shader->SetUniformMat3("pr_matrix", projectionMatrix);
    m_shader->Disable();
    for (auto it = frames.begin(); it != frames.end(); ++it)
    {
      (*it)->OnViewportResize(event.GetWidth(), event.GetHeight());
    }
  }

  void GUIScene::PreRender() const
  {
    m_shader->Enable();
    m_renderer->Begin();
  }

  void GUIScene::Render() const
  {
    for (auto it = frames.begin(); it != frames.end(); ++it)
    {
      (*it)->PreRender(m_renderer, Vec2(0,0));
      (*it)->RenderHandle(m_renderer);
      (*it)->PostRender(m_renderer);
    }
  }

  void GUIScene::PostRender() const
  {
    m_renderer->End();
    m_renderer->Draw();
    m_shader->Disable();
  }

  void GUIScene::Update(float timeElapsed)
  {
    for (auto it = frames.begin(); it != frames.end(); ++it)
    {
      (*it)->UpdateHandle(timeElapsed);
    }
  }

  // Could do checks if a popup is refusing to give request or something
  bool GUIScene::RequestFocus(Component* component)
  {
    // Unfocus the currently focused component
    if(m_focused)
      m_focused->OnUnfocused();

    // Focus the requested one
    if(component && component != m_focused)
      component->OnFocused();

    // TODO: Maybe clean this code up.
    Component* unfocused = m_focused;
    Component* focused = component;

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

    m_focused = component;

    return true;
  }

  void GUIScene::AddFrame(Frame* frame)
  {
    if (frame == nullptr)
    {
      Log::Warning("Cannot add nullptr to frames");

      return;
    }
    frame->Measure();
    frame->MeasureFill(GetWidth(),GetHeight(), 1, true);
    frame->SetGUIScene(this);
    frames.push_back(frame);
    frame->PostConstruction();
  }

  Frame* GUIScene::RemoveFrame(const std::string& name)
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

  Frame* GUIScene::RemoveFrame(Frame* frame)
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

  Frame* GUIScene::GetFrame(const std::string& name)
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
  float GUIScene::GetWidth()
  {
    return RenderCommand::GetViewportWidth();
  }
  float GUIScene::GetHeight()
  {
    return RenderCommand::GetViewportHeight();
  }
}
