#include "Viewport.h"

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
    glViewport(translatedPos.x, Window::GetHeight() - (translatedPos.y+size.size.y), size.size.x, size.size.y);

    sceneManager.Render();

    glViewport(0, 0, Window::GetWidth(), Window::GetHeight());

    guiScene->PreRender();
  }

  SceneManager& Viewport::GetSceneManager()
  {
    return sceneManager;
  }
}
