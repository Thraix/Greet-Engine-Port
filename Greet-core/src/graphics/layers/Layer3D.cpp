#include "Layer3D.h"

namespace Greet {

  Layer3D::Layer3D(Renderer3D* renderer)
    : m_renderer(renderer)
  {
  }

  Layer3D::~Layer3D()
  {
    delete m_renderer;
  }

  void Layer3D::PreRender() const
  {
    m_renderer->Begin();
  }

  void Layer3D::Render() const
  {
    m_renderer->Render();
  }

  void Layer3D::PostRender() const 
  {
    m_renderer->End();
  }

  void Layer3D::Update(float timeElapsed)
  {
    m_renderer->Update(timeElapsed);
  }

  InputControlRequest Layer3D::OnInputChanged(const InputControl* control)
  {
    m_renderer->GetCamera()->OnInputChanged(control);

  }
}
