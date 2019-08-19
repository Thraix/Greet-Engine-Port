#include "GUIScene.h"

#include <graphics/gui/GUIUtils.h>
#include <event/EventDispatcher.h>
#include <graphics/gui/Frame.h>

namespace Greet {

  GUIScene::GUIScene(GUIRenderer* renderer, Shader&& shader)
    : m_renderer(renderer), m_shader(std::move(shader)), projectionMatrix(Mat3::Orthographic(0, Window::GetWidth(), 0, Window::GetHeight()))
  {
    m_focused = nullptr;

    GLint texIDs[32];
    for (int i = 0; i < 32; i++)
    {
      texIDs[i] = i;
    }
    m_shader.Enable();
    m_shader.SetUniformMat3("pr_matrix", projectionMatrix);
    m_shader.SetUniform1iv("textures", 32, texIDs);
    m_shader.Disable();
  }

  void GUIScene::OnEvent(Event& event)
  {
    if(EVENT_IS_CATEGORY(event, INPUT_EVENT))
    {
      if(EVENT_IS_TYPE(event, EventType::KEY_PRESS))
        OnPressed((KeyPressEvent&)event);
      else if(EVENT_IS_TYPE(event, EventType::KEY_RELEASE))
        OnReleased((KeyReleaseEvent&)event);
      else if(EVENT_IS_TYPE(event, EventType::KEY_TYPE))
        OnTyped((KeyTypeEvent&)event);
      else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
        OnMoved((MouseMoveEvent&)event);
      else if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
        OnPressed((MousePressEvent&)event);
      else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
        OnReleased((MouseReleaseEvent&)event);
      else
        Scene::OnEvent(event);
    }
    else
      Scene::OnEvent(event);
  }

  void GUIScene::OnPressed(MousePressEvent& event)
  {
    Vec2 pos = ~projectionMatrix * event.GetPosition();
    MousePressEvent transformedEvent{pos.x,pos.y,event.GetButton()};
    for (auto it = frames.rbegin(); it != frames.rend(); ++it)
    {
      Vec2 pos = transformedEvent.GetPosition() - (*it)->GetMargin().LeftTop();
      if((*it)->IsMouseInside(pos))
      {
        (*it)->OnMousePressed(transformedEvent, pos - (*it)->GetPosition());
        event.AddFlag(EVENT_FOCUSED  | EVENT_HANDLED);
        frames.splice(frames.end(),frames,std::next(it).base());
        return;
      }
    }
    event.AddFlag( transformedEvent.GetFlags());
    RequestFocus(nullptr);
    event.AddFlag(EVENT_UNFOCUSED);
  }

  void GUIScene::OnReleased(MouseReleaseEvent& event)
  {
    if (m_focused != nullptr)
    {
      Vec2 pos = ~projectionMatrix * event.GetPosition();
      MouseReleaseEvent transformedEvent{pos.x,pos.y,event.GetButton()};
      m_focused->MouseReleased(transformedEvent, transformedEvent.GetPosition() - m_focused->GetRealPosition());
      event.AddFlag(EVENT_HANDLED);
    }
  }

  void GUIScene::OnMoved(MouseMoveEvent& event)
  {
    Mat3 projectionInv = ~projectionMatrix;
    Vec2 pos = projectionInv * event.GetPosition();
    Vec2 delta = projectionInv * event.GetDeltaPosition();
    MouseMoveEvent transformedEvent{pos.x,pos.y, delta.x, delta.y};

    if (m_focused != nullptr)
    {
      m_focused->MouseMoved(transformedEvent, transformedEvent.GetPosition() - m_focused->GetRealPosition());
      if(m_focused->UsingMouse())
      {
        event.AddFlag(EVENT_HANDLED);
        return;
      }
    }

    for (auto it = frames.rbegin(); it != frames.rend(); ++it)
    {
      (*it)->OnMouseMoved(transformedEvent, transformedEvent.GetPosition() - (*it)->GetPosition() - (*it)->GetMargin().LeftTop());
    }
  }

  void GUIScene::OnPressed(KeyPressEvent& event)
  {
    if (m_focused != nullptr)
    {
      m_focused->KeyPressed(event);
      event.AddFlag(EVENT_HANDLED);
    }
  }

  void GUIScene::OnReleased(KeyReleaseEvent& event)
  {
    if (m_focused != nullptr)
    {
      m_focused->KeyReleased(event);
      event.AddFlag(EVENT_HANDLED);
    }
  }
  void GUIScene::OnTyped(KeyTypeEvent& event)
  {
    if (m_focused != nullptr)
    {
      m_focused->KeyTyped(event);
      event.AddFlag(EVENT_HANDLED);
    }
  }

  void GUIScene::WindowResize(WindowResizeEvent& event)
  {
    projectionMatrix = Mat3::Orthographic(0, Window::GetWidth(), 0, Window::GetHeight());
    m_shader.Enable();
    m_shader.SetUniformMat3("pr_matrix", projectionMatrix);
    m_shader.Disable();
    for (auto it = frames.begin(); it != frames.end(); ++it)
    {
      (*it)->OnWindowResize(event.GetWidth(), event.GetHeight());
    }
  }

  void GUIScene::Render() const
  {
    m_shader.Enable();
    m_renderer->Begin();
    for (auto it = frames.begin(); it != frames.end(); ++it)
    {
      (*it)->PreRender(m_renderer, Vec2(0,0));
      (*it)->RenderHandle(m_renderer);
      (*it)->PostRender(m_renderer);
    }

    m_renderer->End();
    m_renderer->Draw();
    m_shader.Disable();
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
    return Window::GetWidth();
  }
  float GUIScene::GetHeight()
  {
    return Window::GetHeight();
  }
}
