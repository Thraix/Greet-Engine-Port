#pragma once

#include <graphics/shaders/Shader.h>
#include <graphics/Renderable.h>
#include <graphics/renderers/BatchRenderer.h>
#include <graphics/renderers/Renderer2D.h>
#include <graphics/layers/Scene.h>

namespace Greet {

  class Layer : public Scene
  {
    protected:
      Ref<Renderer2D> renderer;
      std::vector<Ref<Renderable>> renderables;

      Mat3 projectionMatrix;
    public:
      Layer(const Ref<Renderer2D>& renderer, const Mat3& projectionMatrix)
        : renderer(renderer), projectionMatrix(projectionMatrix)
      {
        renderer->GetShader()->Enable();
        renderer->GetShader()->SetUniformMat3("uProjectionMatrix", projectionMatrix);
        renderer->GetShader()->SetUniformMat3("uViewMatrix", Mat3::Scale(1));
        renderer->GetShader()->Disable();
      }

      virtual ~Layer()
      {}

      virtual void Add(const Ref<Renderable>& renderable)
      {
        renderables.push_back(renderable);
      }

      virtual void Render() const override
      {
        renderer->Begin();
        setUniforms();
        uint size = renderables.size();
        for (uint i = 0; i < size; i++)
        {
          renderables[i]->Begin(renderer);
          renderables[i]->Render(renderer);
          renderables[i]->End(renderer);
        }
        renderer->End();
        renderer->Flush();
      }

      virtual void Update(float timeElapsed) override
      {
        uint size = renderables.size();
        for (uint i = 0; i < size; i++)
          renderables[i]->Update(timeElapsed);
      }

      void SetProjectionMatrix(Mat3 amProjectionMatrix)
      {
        projectionMatrix = amProjectionMatrix;
        renderer->GetShader()->Enable();
        renderer->GetShader()->SetUniformMat3("uProjectionMatrix", projectionMatrix);
        renderer->GetShader()->Disable();
      }

      virtual void ViewportResize(float x, float y, float width, float height) {}
      virtual void ViewportResize(ViewportResizeEvent& event) override {}
      Mat3 GetProjectionMatrix() const { return projectionMatrix; };
      inline const Ref<Shader>& GetShader() const { return renderer->GetShader(); }
      inline uint Size() const { return renderables.size(); }


    protected:
      virtual void setUniforms() const {}
  };
}
