#pragma once

#include <vector>
#include <graphics/layers/Scene.h>
#include <graphics/SceneManager.h>
#include <event/Event.h>

namespace Greet {
  class GlobalSceneManager
  {
    DELETE_CONSTRUCTOR_DESTRUCTOR(GlobalSceneManager);

    private:
      static SceneManager* sceneManager;
    public:
      static void Create();
      static void Destroy();
      static SceneManager& GetSceneManager();
  };
}
