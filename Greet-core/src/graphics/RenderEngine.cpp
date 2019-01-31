#include "RenderEngine.h"

#include <logging/Log.h>

namespace Greet {

  std::map<std::string, Scene*> RenderEngine::m_scenes2d;
  std::map<std::string, Scene*> RenderEngine::m_scenes3d;
  Scene* RenderEngine::focusedScene = nullptr;

  void RenderEngine::Add2DScene(Scene* scene, const std::string& name)
  {
    if (scene == NULL)
    {
      Log::Error("Trying to add Scene to RenderEngine but it is NULL.");
      return;
    }
    m_scenes2d.emplace(name, scene);
  }

  void RenderEngine::Add3DScene(Scene* scene, const std::string& name)
  {
    if (scene == NULL)
    {
      Log::Error("Trying to add Renderer3D to RenderEngine but it is NULL.");
      return;
    }
    m_scenes3d.emplace(name, scene);
  }

  Scene* RenderEngine::Remove2DScene(const std::string& name)
  {
    auto it = m_scenes2d.find(name);
    m_scenes2d.erase(it);
    return it->second;
  }

  Scene* RenderEngine::Remove3DScene(const std::string& name)
  {
    auto it = m_scenes3d.find(name);
    m_scenes3d.erase(it);
    return it->second;
  }

  Scene* RenderEngine::Get2DScene(const std::string& name)
  {
    auto it = m_scenes2d.find(name);
    return it->second;
  }

  Scene* RenderEngine::Get3DScene(const std::string& name)
  {
    auto it = m_scenes3d.find(name);
    return it->second;
  }

  void RenderEngine::Render()
  {
    for (auto it = m_scenes3d.begin(); it != m_scenes3d.end(); it++)
    {
      it->second->PreRender();
      it->second->Render();
      it->second->PostRender();
    }

    for (auto it = m_scenes2d.begin(); it != m_scenes2d.end(); it++)
    {
      it->second->PreRender();
      it->second->Render();
      it->second->PostRender();
    }
  }

  void RenderEngine::Update(float timeElapsed)
  {
    for (auto it = m_scenes3d.begin(); it != m_scenes3d.end(); it++)
    {
      it->second->PreUpdate(timeElapsed);
      it->second->Update(timeElapsed);
      it->second->PostUpdate(timeElapsed);
    }

    for (auto it = m_scenes2d.begin(); it != m_scenes2d.end(); it++)
    {
      it->second->PreUpdate(timeElapsed);
      it->second->Update(timeElapsed);
      it->second->PostUpdate(timeElapsed);
    }
  }

  void RenderEngine::OnEvent(Event& event)
  {
    if(focusedScene)
    {
      focusedScene->OnEvent(event);
      if(event.GetFlags() & EVENT_UNFOCUSED)
        focusedScene = nullptr;
      if(event.GetFlags()  & EVENT_HANDLED)
        return;
    }

    // The last to be rendered will be on top so check input there first.
    for (auto it = m_scenes2d.rbegin(); it != m_scenes2d.rend(); it++)
    {
      if(it->second == focusedScene)
        continue;
      it->second->OnEvent(event);
      // Generally when you request focus you also handle the input.
      // But there might be a case where that is not the case.
      if(event.GetFlags() & EVENT_FOCUSED)
        focusedScene = it->second;
      if(event.GetFlags() & EVENT_HANDLED)
        return;
    }

    for (auto it = m_scenes3d.rbegin(); it != m_scenes3d.rend(); it++)
    {
      if(it->second == focusedScene)
        continue;
      it->second->OnEvent(event);
      // Generally when you request focus you also handle the input.
      // But there might be a case where that is not the case.
      if(event.GetFlags() & EVENT_FOCUSED)
        focusedScene = it->second;
      if(event.GetFlags() & EVENT_HANDLED)
        return;
    }
  }
}
