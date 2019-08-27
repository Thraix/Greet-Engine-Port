#include "Viewport.h"

#include <graphics/RenderCommand.h>

namespace Greet
{

  REGISTER_COMPONENT_DEFINITION(Viewport);

  Viewport::Viewport(const XMLObject& xmlObject, Component* parent)
    : Component(xmlObject, parent)
  {

  }

  void Viewport::Update(float timeElapsed)
  {
    sceneManager.Update(timeElapsed);
  }

  void Viewport::Render(GUIRenderer* renderer) const
  {
    guiScene->PostRender();

    Vec2 translatedPos = renderer->GetMatrix() * pos;

    RenderCommand::PushViewportStack(translatedPos.x, translatedPos.y, size.size.w, size.size.h);

    sceneManager.Render();

    RenderCommand::PopViewportStack();

    guiScene->PreRender();
  }

  SceneManager& Viewport::GetSceneManager()
  {
    return sceneManager;
  }
}
