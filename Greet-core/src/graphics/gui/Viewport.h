#pragma once

#include <graphics/gui/Component.h>
#include <graphics/SceneManager.h>

namespace Greet
{
  class Viewport : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(Viewport);

      SceneManager sceneManager;
    public:
      Viewport(const XMLObject& xmlObject, Component* parent);

      void Update(float timeElapsed) override;
      void Render(GUIRenderer* renderer) const override;

      const SceneManager& GetSceneManager() const;
      SceneManager& GetSceneManager();
  };
} 

