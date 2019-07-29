#pragma once

#include <vector>
#include <graphics/layers/Scene.h>
#include <event/Event.h>

namespace Greet {
  class RenderEngine
  {
    typedef std::pair<std::string, Scene*> SceneElement; 
    private:
      // they are the same but 3d scenes are always rendered first
    static std::vector<SceneElement> m_scenes2d;
    static std::vector<SceneElement> m_scenes3d;
    static Scene* focusedScene;

    public:
      static void Add2DScene(Scene* scene, const std::string& name);
      static void Add3DScene(Scene* scene, const std::string& name);

      static Scene* Remove2DScene(const std::string& name);
      static Scene* Remove3DScene(const std::string& name);
      static Scene* Get2DScene(const std::string& name);
      static Scene* Get3DScene(const std::string& name);
      static void Render();
      static void Update(float timeElapsed);
      static void OnEvent(Event& event);
  };
}
