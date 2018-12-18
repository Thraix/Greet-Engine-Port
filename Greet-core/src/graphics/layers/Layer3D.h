#pragma once

#include <graphics/renderers/Renderer3D.h>
#include <graphics/layers/Scene.h>

namespace Greet {
  class Layer3D : public Scene
  {
    protected:
      Renderer3D* m_renderer;
    public:
      Layer3D(Renderer3D* renderer);
      virtual ~Layer3D();
      virtual void PreRender() const override;
      virtual void Render() const override;
      virtual void PostRender() const override;
      virtual void Update(float timeElapsed) override;
      virtual InputControlRequest OnInputChanged(const InputControl* control) override;
      const Renderer3D* GetRenderer() const { return m_renderer;}
      void SetRenderer(Renderer3D* renderer) { m_renderer = renderer;}
  };
}
