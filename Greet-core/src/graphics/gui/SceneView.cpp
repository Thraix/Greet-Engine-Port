#include "SceneView.h"

#include <graphics/RenderCommand.h>
#include <input/Input.h>
#include <event/ViewportEvent.h>

namespace Greet
{

  REGISTER_COMPONENT_DEFINITION(SceneView);

  SceneView::SceneView(const XMLObject& xmlObject, Component* parent)
    : Component(xmlObject, parent)
  {}

  void SceneView::Update(float timeElapsed)
  {
    Vec2 translatedPos = GetRealPosition();
    RenderCommand::PushViewportStack(GetRealPosition(), GetSize());

    sceneManager.Update(timeElapsed);

    RenderCommand::PopViewportStack();
  }

  void SceneView::Render(GUIRenderer* renderer) const
  {
    guiScene->PostRender();
    RenderCommand::PushViewportStack(GetRealPosition(), GetSize());

    sceneManager.Render();

    RenderCommand::PopViewportStack();
    guiScene->PreRender();
  }

  void SceneView::OnEventHandler(Event& event, const Vec2& componentPos)
  {
    RenderCommand::PushViewportStack(componentPos, GetSize());
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

  void SceneView::OnMeasured()
  {
    if(width.size <= 0 || height.size <= 0)
      return;
    Vec2 realPos = GetRealPosition();
    RenderCommand::PushViewportStack(realPos, GetSize());
    ViewportResizeEvent event{realPos, GetSize()};
    sceneManager.OnEvent(event);
    RenderCommand::PopViewportStack();
  }

  SceneManager& SceneView::GetSceneManager()
  {
    return sceneManager;
  }
}
