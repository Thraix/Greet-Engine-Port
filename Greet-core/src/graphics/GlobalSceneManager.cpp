#include "GlobalSceneManager.h"

#include <logging/Log.h>
#include <utils/HotSwapping.h>

namespace Greet {

  SceneManager* GlobalSceneManager::sceneManager;

  SceneManager& GlobalSceneManager::GetSceneManager()
  {
    return *sceneManager;
  }

  void GlobalSceneManager::Create()
  {
    sceneManager = new SceneManager();
  }

  void GlobalSceneManager::Destroy()
  {
    delete sceneManager;
  }
}
