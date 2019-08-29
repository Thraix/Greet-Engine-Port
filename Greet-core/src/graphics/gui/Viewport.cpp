#include "Viewport.h"

#include <graphics/RenderCommand.h>
#include <input/Input.h>

namespace Greet
{

  REGISTER_COMPONENT_DEFINITION(Viewport);

  Viewport::Viewport(const XMLObject& xmlObject, Component* parent)
    : Component(xmlObject, parent)
  {
    m_isFocusable = true;
  }

  void Viewport::Update(float timeElapsed)
  {
    Vec2 translatedPos = GetRealPosition();
    RenderCommand::PushViewportStack(translatedPos.x, translatedPos.y, size.size.w, size.size.h);

    sceneManager.Update(timeElapsed);

    RenderCommand::PopViewportStack();
  }

  void Viewport::Render(GUIRenderer* renderer) const
  {
    guiScene->PostRender();
    Vec2 translatedPos = renderer->GetMatrix() * pos;
    RenderCommand::PushViewportStack(translatedPos, size.size);

    sceneManager.Render();

    RenderCommand::PopViewportStack();
    guiScene->PreRender();
  }

  void Viewport::OnEventHandler(Event& event, const Vec2& componentPos)
  {
    RenderCommand::PushViewportStack(componentPos, size.size);
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      guiScene->RequestFocus(this);
      MousePressEvent transformedEvent{Input::GetMousePos(), static_cast<MousePressEvent&>(event).GetButton()};
      sceneManager.OnEvent(transformedEvent);
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      MouseMoveEvent transformedEvent{Input::GetMousePos(), Input::GetMousePosDelta()};
      sceneManager.OnEvent(transformedEvent);
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
    {
      MouseReleaseEvent transformedEvent{Input::GetMousePos(), static_cast<MouseReleaseEvent&>(event).GetButton()};
      sceneManager.OnEvent(transformedEvent);
    }
    else
      sceneManager.OnEvent(event);
    RenderCommand::PopViewportStack();
  }

  SceneManager& Viewport::GetSceneManager()
  {
    return sceneManager;
  }
}
