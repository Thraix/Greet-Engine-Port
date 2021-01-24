#pragma once

#include <ecs/ECSManager.h>
#include <ecs/Entity.h>
#include <graphics/Skybox.h>
#include <graphics/layers/Scene.h>
#include <graphics/renderers/BatchRenderer.h>
#include <utils/MetaFile.h>

namespace Greet
{
  class ECSScene : public Greet::Scene
  {
    private:
      Ref<ECSManager> manager;
      Ref<BatchRenderer> renderer2d;

    public:
      ECSScene();
      ECSScene(const std::string& scenePath);
      virtual ~ECSScene();

      void LoadEntity(const MetaFile& meta);

      Entity AddEntity(const std::string& tag);
      void RemoveEntity(const Entity& entity);
      void RemoveEntity(EntityID entity);

      void Render() const override;
      void Render2D() const;
      void Render3D() const;
      virtual void UpdateBefore(float timeElapsed) {}
      void Update(float timeElapsed) override;
      virtual void UpdateAfter(float timeElapsed) {}
      void OnEvent(Event& event) override;

    private:
      template <typename T>
      void LoadComponent(Entity& entity, const MetaFile& meta, const std::string& componentName)
      {
        const std::vector<MetaFileClass>& metaClass = meta.GetMetaClass(componentName);
        if(metaClass.size() > 0)
          entity.AddComponent<T>(metaClass[0]);
      }

      // Loads external components not handled by the engine.
      // Gives the developer more control over what components can be used
      virtual void LoadExtComponents(Entity& entity, const MetaFile& metaClass) {}
  };
}
